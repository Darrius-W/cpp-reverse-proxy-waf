# C++ Reverse Proxy WAF

A lightweight reverse proxy Web Application Firewall (WAF) built from the ground up in C++.

The project sits between a client and a backend server, intercepting HTTP traffic and inspecting requests before deciding whether to block or forward them.

## Overview

The WAF is designed around a modular, pipeline-based architecture.

```text
Client
   |
   v
+-----------+
|    WAF    |
+-----------+
   |
   +----> Inspect Request
   |
   +----> Block Malicious Request
   |
   +----> Forward Legitimate Request
   |
   v
Backend Server
```

The primary goal of the project is to explore the intersection of **systems programming, networking, and application security** by implementing the core components of a reverse proxy WAF in C++.

## Goals

* Accept TCP connections from clients
* Parse HTTP requests
* Process requests through a security pipeline
* Detect common web attacks
* Block malicious requests
* Forward legitimate requests to a backend server
* Log security-related events
* Build automated security and integration tests

## Architecture

The WAF is divided into several major subsystems:

* Network Layer
* HTTP Layer
* Security Pipeline
* Rule Engine
* Proxy Layer
* Logging
* Configuration

The intended request flow is:

```text
Client
  |
  v
Network Layer
  |
  v
HTTP Parser
  |
  v
Security Pipeline
  |
  v
Rule Engine
  |
  +---------> BLOCK
  |
  +---------> ALLOW
                  |
                  v
             Proxy Layer
                  |
                  v
            Backend Server
```

See [`docs/architecture.md`](docs/architecture.md) for the detailed architecture.

## Technology

* **C++** - Core WAF implementation
* **CMake** - Build system
* **MSVC** - C++ compiler
* **Python** - Integration testing and development utilities
* **Git/GitHub** - Version control

## Project Structure

```text
cpp-waf/
│
├── CMakeLists.txt
├── README.md
├── .gitignore
│
├── docs/
│   ├── requirements.md
│   ├── architecture.md
│   ├── workflow.md
│   ├── roadmap.md
│   └── components.md
│
├── include/
├── src/
├── tests/
└── scripts/
```

## Project Status

**Status: 🚧 In Development**

The project is currently in the initial development stage. The architecture, requirements, workflow, components, and development roadmap have been established.

Current focus:

* Project architecture
* Core networking layer
* HTTP request handling
* Security pipeline design

## Development Roadmap

The MVP is planned to progress through the following stages:

1. TCP networking
2. HTTP request parsing
3. Security pipeline
4. Security rule engine
5. Reverse proxy functionality
6. Logging
7. Automated testing
8. MVP validation

See [`docs/roadmap.md`](docs/roadmap.md) for the detailed development plan.

## Documentation

Project documentation can be found in the [`docs/`](docs/) directory.

* [`requirements.md`](docs/requirements.md) - System requirements and MVP scope
* [`architecture.md`](docs/architecture.md) - System architecture and subsystem responsibilities
* [`workflow.md`](docs/workflow.md) - Request processing workflows
* [`components.md`](docs/components.md) - Component responsibilities and relationships
* [`roadmap.md`](docs/roadmap.md) - Development milestones

## Future Improvements

Potential features beyond the MVP include:

* HTTPS/TLS termination
* HTTP/2 support
* Rate limiting
* IP reputation
* Asynchronous I/O
* Additional security rules
* Machine learning-based threat detection
* Performance benchmarking
* Configuration files
* Improved logging and monitoring

## Learning Objectives

This project is intended to strengthen practical understanding of:

* C++ systems programming
* TCP networking
* HTTP protocol handling
* Resource and memory management
* Reverse proxy architecture
* Web application security
* Modular software design
* Automated security testing
