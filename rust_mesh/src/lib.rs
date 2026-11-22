use std::collections::HashMap;
use std::time::{Duration, Instant};

pub struct MeshNode {
    pub node_id: u32,
    peers: HashMap<u32, Instant>,
    timeout_duration: Duration,
}

impl MeshNode {
    pub fn new(node_id: u32, timeout_secs: u64) -> Self {
        Self {
            node_id,
            peers: HashMap::new(),
            timeout_duration: Duration::from_secs(timeout_secs),
        }
    }

    pub fn update_heartbeat(&mut self, peer_id: u32) {
        self.peers.insert(peer_id, Instant::now());
    }

    pub fn active_peer_count(&self) -> usize {
        let now = Instant::now();
        self.peers
            .values()
            .filter(|&&last_seen| now.duration_since(last_seen) <= self.timeout_duration)
            .count()
    }

    pub fn check_mesh_health(&self) -> Vec<u32> {
        let now = Instant::now();
        self.peers
            .iter()
            .filter_map(|(&peer_id, &last_seen)| {
                if now.duration_since(last_seen) > self.timeout_duration {
                    Some(peer_id)
                } else {
                    None
                }
            })
            .collect()
    }

    pub fn prune_timed_out_peers(&mut self) -> usize {
        let stale_ids = self.check_mesh_health();
        let removed = stale_ids.len();
        for id in stale_ids {
            self.peers.remove(&id);
        }
        removed
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::thread;

    #[test]
    fn test_mesh_heartbeat_and_pruning() {
        let mut node = MeshNode::new(1, 1);
        node.update_heartbeat(2);
        assert_eq!(node.active_peer_count(), 1);

        thread::sleep(Duration::from_millis(1100));
        let disconnected = node.check_mesh_health();
        assert!(disconnected.contains(&2));

        let removed = node.prune_timed_out_peers();
        assert_eq!(removed, 1);
        assert_eq!(node.active_peer_count(), 0);
    }
}
