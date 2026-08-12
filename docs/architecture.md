# C++ Reverse Proxy WAF Architecture

## 1. Architecture Overview

The WAF is designed as a layered, pipeline-based reverse proxy.

Its primary responsibility is to sit between a client and a backend web application, inspect incoming HTTP requests, determine whether they are legitimate or malicious, and either block the request or forward it to the backend.

The system is divided into several major subsystems:

1. Network Layer
2. HTTP Layer
3. Security Pipeline
4. Rule Engine
5. Proxy Layer
6. Logging
7. Configuration

The overall request flow is:

```text
                    Client
                       |
                       | TCP / HTTP
                       v
                +--------------+
                | Network      |
                | Layer        |
                +--------------+
                       |
                       | Raw HTTP data
                       v
                +--------------+
                | HTTP         |
                | Parser       |
                +--------------+
                       |
                       | HTTPRequest
                       v
                +--------------+
                | Security     |
                | Pipeline     |
                +--------------+
                       |
                       v
                +--------------+
                | Rule Engine  |
                +--------------+
                  /          \
                 /            \
                v              v
             BLOCK            ALLOW
                |               |
                v               v
          HTTP 403          Proxy Layer
                                |
                                v
                         Backend Server
                                |
                                v
                         HTTP Response
                                |
                                v
                              Client
```

The architecture intentionally separates network communication, HTTP processing, security inspection, and proxy functionality. This allows each subsystem to be developed and tested independently.

---

## 2. Network Layer

### Purpose

The Network Layer is responsible for establishing and maintaining TCP communication between clients and the WAF.

The networking concepts used here build upon previous experience implementing TCP socket communication in a real-time chat application.

For this project, the focus is not on reinventing socket communication, but on integrating the networking layer into a larger HTTP and security-processing pipeline.

### Responsibilities

* Create and configure the listening socket.
* Bind the WAF to a configured port.
* Listen for incoming connections.
* Accept client connections.
* Receive raw network data.
* Send responses back to clients.
* Manage connection lifecycle.

### Does Not Handle

* HTTP parsing
* Security rules
* Attack detection
* Backend request forwarding

### Conceptual Components

```text
Network Layer
├── TcpServer
├── Socket
└── Connection
```

---

## 3. HTTP Layer

### Purpose

The HTTP Layer converts raw network data into structured HTTP requests that can be processed by the security pipeline.

### Responsibilities

* Parse the HTTP request line.
* Extract the HTTP method.
* Extract the request path.
* Parse HTTP headers.
* Extract the request body.
* Construct structured HTTP request objects.
* Construct HTTP responses.

### Input

Raw HTTP data received from the Network Layer.

Example:

```text
GET /login?id=123 HTTP/1.1
Host: localhost
User-Agent: curl
```

### Output

A structured `HTTPRequest` object.

Conceptually:

```text
HTTPRequest
├── method
├── path
├── version
├── headers
└── body
```

### Conceptual Components

```text
HTTP Layer
├── HTTPParser
├── HTTPRequest
└── HTTPResponse
```

The HTTP Layer does not determine whether a request is malicious. Its responsibility is to correctly interpret the request.

---

## 4. Security Pipeline

### Purpose

The Security Pipeline processes a parsed HTTP request through a sequence of security checks.

The pipeline-based architecture allows security checks to be added, removed, or reordered without significantly changing the rest of the WAF.

### Request Flow

```text
HTTPRequest
     |
     v
+----------------+
| Security Check |
+----------------+
     |
     v
+----------------+
| Security Check |
+----------------+
     |
     v
+----------------+
| Security Check |
+----------------+
     |
     v
 Security Decision
```

Each stage receives information about the request and produces a result that can be used by the next stage.

### Possible Pipeline Stages

```text
Request
   |
   v
Request Validation
   |
   v
IP / Access Rules
   |
   v
SQL Injection Detection
   |
   v
XSS Detection
   |
   v
Path Traversal Detection
   |
   v
Final Security Decision
```

The MVP will initially contain only a small number of rules. Additional stages can be added as the project evolves.

---

## 5. Rule Engine

### Purpose

The Rule Engine contains the individual security rules used to evaluate requests.

The Rule Engine should be modular so that new detection rules can be added without modifying the core networking or proxy code.

### Responsibilities

* Register security rules.
* Execute enabled rules.
* Evaluate requests.
* Produce security decisions.
* Provide information about triggered rules.

### Conceptual Structure

```text
RuleEngine
    |
    +── Rule
    |
    +── SQLInjectionRule
    |
    +── XSSRule
    |
    +── PathTraversalRule
```

A rule should conceptually answer:

```text
Does this request violate this security rule?

        |
        +---- NO ----> Continue
        |
        +---- YES ---> BLOCK
```

### Important Design Principle

Individual rules should not be responsible for:

* Managing sockets
* Sending HTTP responses
* Forwarding requests
* Managing the client connection

They should primarily be responsible for analyzing request data and reporting their findings.

---

## 6. Security Decision

After the request passes through the security pipeline, the WAF produces a decision.

```text
Security Pipeline
       |
       v
+-------------------+
| Security Decision |
+-------------------+
       |
       +------> ALLOW
       |
       +------> BLOCK
```

For the MVP, the primary decisions are:

```text
ALLOW
BLOCK
```

A blocked request should result in an appropriate HTTP error response, such as:

```text
HTTP/1.1 403 Forbidden
```

An allowed request continues to the Proxy Layer.

---

## 7. Proxy Layer

### Purpose

The Proxy Layer forwards legitimate HTTP requests from the WAF to the configured backend server.

The WAF therefore acts as an intermediary:

```text
Client
   |
   v
 WAF
   |
   v
Backend
```

### Responsibilities

* Establish communication with the backend.
* Forward allowed requests.
* Receive backend responses.
* Return backend responses to the client.

### Does Not Handle

* Attack detection
* Security rule evaluation
* Initial HTTP security inspection

Those responsibilities belong to the Security Pipeline and Rule Engine.

---

## 8. Logging

### Purpose

The Logging subsystem records important events occurring within the WAF.

### Possible Events

```text
Client connected
Request received
Request allowed
Request blocked
Security rule triggered
Backend connection failed
Malformed request received
WAF shutdown
```

Example:

```text
[INFO] 192.168.1.20 GET /index.html ALLOWED

[WARN] 192.168.1.20 GET /search?q=<payload> BLOCKED
       Rule: XSSRule
```

The Logger should record security events without being responsible for making security decisions.

---

## 9. Configuration

### Purpose

The Configuration subsystem provides runtime settings to the WAF.

Potential configuration values include:

```text
Listen Port
Backend Address
Backend Port
Enabled Rules
Logging Configuration
```

For the MVP, configuration may initially be hardcoded or provided through command-line arguments. A configuration file can be introduced later.

---

## 10. Overall Data Flow

The complete request lifecycle is:

```text
1. Client establishes TCP connection
              |
              v
2. Network Layer receives raw data
              |
              v
3. HTTP Parser converts raw data into HTTPRequest
              |
              v
4. Security Pipeline processes HTTPRequest
              |
              v
5. Rule Engine evaluates security rules
              |
          +---+---+
          |       |
        BLOCK    ALLOW
          |       |
          v       v
      HTTP 403  Proxy Layer
                  |
                  v
            Backend Server
                  |
                  v
            Backend Response
                  |
                  v
               Client
```

Security-related events are sent to the Logging subsystem throughout this process.

---

## 11. Component Responsibilities

| Component      | Responsible For                 | Not Responsible For  |
| -------------- | ------------------------------- | -------------------- |
| `TcpServer`    | Accepting connections           | Parsing HTTP         |
| `Socket`       | Low-level network communication | Security decisions   |
| `HTTPParser`   | Parsing HTTP                    | Detecting attacks    |
| `HTTPRequest`  | Representing request data       | Processing requests  |
| `Pipeline`     | Executing security stages       | Managing sockets     |
| `RuleEngine`   | Executing security rules        | Sending responses    |
| `Rule`         | Detecting a specific condition  | Managing connections |
| `ProxyHandler` | Forwarding requests/responses   | Detecting attacks    |
| `Logger`       | Recording events                | Making decisions     |
| `Config`       | Providing settings              | Processing requests  |

---

## 12. Architectural Principles

### Separation of Responsibilities

Each subsystem should have a clearly defined responsibility.

### Modularity

Security rules should be independently replaceable or expandable.

### Testability

Major components should be testable independently where practical.

### Extensibility

The architecture should allow additional security rules and processing stages to be introduced without major changes to the networking or proxy layers.

### Resource Awareness

Because the project is implemented in C++, resource ownership, memory management, socket lifecycle, and error handling should be explicitly considered during implementation.
