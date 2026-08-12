# Requirements

## Project Overview

A lightweight reverse proxy Web Application Firewall (WAF) written in C++ that intercepts HTTP traffic, analyzes requests using a modular security pipeline, blocks malicious requests, and forwards legitimate traffic to a backend server.

---

## Goals

- Accept HTTP requests
- Parse HTTP traffic
- Detect malicious requests
- Block malicious traffic
- Forward safe traffic
- Log security events

---

## Non-Goals (MVP)

- HTTPS/TLS termination
- HTTP/2 support
- Web dashboard
- Machine learning detection
- Distributed deployment

---

## Actors

### Client
Sends HTTP requests.

### WAF
Inspects requests and decides whether to allow or block.

### Backend Server
Processes legitimate requests.

### Administrator
Starts and configures the WAF.

---

## Functional Requirements

FR-1: Accept TCP connections.

FR-2: Parse HTTP requests.

FR-3: Evaluate security rules.

FR-4: Block malicious requests.

FR-5: Forward legitimate requests.

FR-6: Log events.

---

## Non-Functional Requirements

- Written in modern C++
- Modular architecture
- Easily extensible rules
- Portable using CMake