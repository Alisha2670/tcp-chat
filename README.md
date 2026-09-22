# Concurrent TCP Chat System in C

## 1. Project Proposal & Overview
A lightweight, concurrent terminal-based communication system built in C for Linux environments. The application enables multiple clients to communicate in real-time through a central coordinator (TCP server) that routes messages, manages user states, and handles concurrency using POSIX threads.

---

## 2. Problem Statement
In distributed computing, direct client-to-client communication can become complex to coordinate, secure, and log. This project solves that problem by implementing a centralized TCP client-server architecture where:
- A central server manages all active user connections.
- Communication is coordinated without direct client-to-client socket binding.
- Concurrency is managed cleanly using multi-threading and mutex synchronization.
- Server state and user events are preserved via logging and graceful signal handling.

---

## 3. System Architecture & Flow

              ┌──────────────────────┐
              │     TCP SERVER       │
              │                      │
              │  Client Manager      │
              │  Message Router      │
              │  User Manager        │
              │  Chat Logger         │
              └──────┬───────┬───────┘
                     │       │
              ┌──────┘       └──────┐
              ↓                     ↓
        ┌───────────┐         ┌───────────┐
        │  Client A │         │  Client B │
        │  Alisha   │         │  Rahul    │
        └───────────┘         └───────────┘
                  \
                   \           ┌───────────┐
                    └─────────►│  Client C │
                               │  Priya    │
                               └───────────┘

### Complete Execution Flow:

                  START SERVER
                       │
                       ↓
                  socket()
                       │
                       ↓
                    bind()
                       │
                       ↓
                   listen()
                       │
                       ↓
                Wait for clients
                       │
                       ↓
                    accept()
                       │
                       ↓
               Create client handler (pthread_create)
                       │
                       ↓
              Username registration
                       │
                       ↓
             Add to client list (mutex locked)
                       │
                       ↓
              ┌─────────────────┐
              │ Receive message │
              └────────┬────────┘
                       ↓
                 Parse message
                       │
            ┌──────────┼──────────┐
            ↓          ↓          ↓
        Normal       /msg       /users
        message       │           │
            ↓         ↓           ↓
       Broadcast    Private     User list
            │        message      │
            └────────┼────────────┘
                     ↓
                  Continue
                     │
                     ↓
                   /quit
                     │
                     ↓
              Remove client (mutex locked)
                     │
                     ↓
                Close socket

---

## 4. Key Goals & Learning Objectives
- Socket Programming: Master socket(), bind(), listen(), accept(), connect(), send(), and recv().
- Concurrency & Threads: Utilize POSIX Threads (pthread) to handle simultaneous client connections.
- Synchronization: Prevent race conditions on the shared client list using mutexes (pthread_mutex_t).
- Signal Handling: Handle SIGINT (Ctrl+C) for graceful server deallocation and resource cleanup.
- File I/O: Implement persistent server-side chat event logging into logs/chat.log.

---

## 5. Directory Structure
tcp-chat/
├── src/            # Implementation files (.c)
├── include/        # Header files (.h)
├── logs/           # Chat logs (.log files)
├── Makefile        # Build automation
├── .gitignore      # Ignored binaries and build artifacts
├── LICENSE         # MIT License
└── README.md       # Project proposal and documentation

---

## 6. Build & Run Instructions

### Prerequisites
- GCC compiler
- POSIX-compliant OS (Linux / Ubuntu WSL)
- Make utility

### Compilation
make

### Running the Server
./bin/server 8080

### Running a Client
./bin/client 127.0.0.1 8080

---

## 7. Supported Commands
- /help : List all available commands.
- /users : Display all currently online users.
- /msg <user> <message> : Send a private message to a specific user.
- /quit : Disconnect cleanly from the server.
