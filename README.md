# Connix

**Connix** is a lightweight, CLI-first networking client for **REST APIs, TCP/UDP, and Unix sockets**.  
Built in modern C++, it provides a unified interface for interacting with multiple protocols from a single tool.

---

## Features

- Unified interface for:
  - HTTP/REST
  - TCP / UDP
  - Unix Domain Sockets
- Lightweight, fast, and composable
- Reusable **core library** for integration
- Optional **Qt/QML GUI**
- Cross-platform: Windows, Linux

---

## Architecture

- `connix` — CLI application  
- `connix-core` — core static library  
- `connix-gui` — optional GUI client  

---

## Tech Stack

- Language: C++20
- Build: CMake
- Package Manager: Conan v2
- Testing: Google Test
- GUI: Qt / QML

---

## License

MIT
