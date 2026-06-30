# A Custom In-Memory Key-Value Store (Redis-lite)

A long-running server process that manages state over time. To test my understanding of shared state, interior mutability, and network I/O.

- **The Goal**: Build a TCP server that listens for incoming text commands to store and retrieve data.

- **Skills learned**: std::net::TcpListener, handling concurrent connections, and safe shared state using Arc<Mutex<T>> or Arc<RwLock<T>>.

- **Core Requirements**:
Support three text-based commands over a TCP socket:
'SET key value'
'GET key'
'DELETE key'

Make it multi-threaded so multiple clients can connect simultaneously without blocking each other.
- **Bonus Stretch**: Implement an expiration mechanism (e.g., SET key value EX 10 deletes the key after 10 seconds) using a background worker thread.
