# Components

This document defines the major software components of the WAF and their responsibilities. Components are organized according to the system architecture defined in `architecture.md`.

The purpose of this document is to establish clear boundaries between components before implementation begins.

---

## 1. Network Layer

The Network Layer manages communication between clients and the WAF.

### TcpServer

**Purpose:**
Manage the WAF's listening server and accept incoming client connections.

**Responsibilities:**

* Initialize the server socket.
* Bind to the configured address and port.
* Listen for incoming connections.
* Accept client connections.
* Manage the lifecycle of client connections.

**Input:**

* Client TCP connection attempts.

**Output:**

* Established client connection.
* Raw request data.

**Does Not Handle:**

* HTTP parsing.
* Security rule evaluation.
* Request forwarding.

**Dependencies:**

* `Socket`
* `Config`

---

### Socket

**Purpose:**
Provide an abstraction around the operating system's socket functionality.

**Responsibilities:**

* Create sockets.
* Bind sockets.
* Listen for connections.
* Accept connections.
* Send data.
* Receive data.
* Close sockets.

**Input:**

* Network operations requested by higher-level components.

**Output:**

* Network data.
* Connection status.

**Does Not Handle:**

* HTTP semantics.
* Security inspection.
* Request routing.

---

### Connection

**Purpose:**
Represent and manage an individual client connection.

**Responsibilities:**

* Maintain the connection state.
* Receive request data.
* Send response data.
* Handle connection shutdown.

**Input:**

* Raw network data.

**Output:**

* Raw HTTP request data.
* HTTP response data sent to the client.

**Dependencies:**

* `Socket`

---

## 2. HTTP Layer

The HTTP Layer converts raw network data into structured HTTP objects.

### HTTPParser

**Purpose:**
Convert raw HTTP request data into a structured `HTTPRequest`.

**Responsibilities:**

* Parse the request line.
* Extract the HTTP method.
* Extract the request path.
* Extract the HTTP version.
* Parse headers.
* Parse the request body.
* Detect malformed HTTP requests.

**Input:**

* Raw HTTP request bytes.

**Output:**

* `HTTPRequest`
* Parsing errors when applicable.

**Does Not Handle:**

* Security decisions.
* Request forwarding.
* Network connection management.

**Dependencies:**

* `HTTPRequest`

---

### HTTPRequest

**Purpose:**
Represent a parsed HTTP request.

**Contains:**

* HTTP method.
* Request path.
* HTTP version.
* Headers.
* Query parameters.
* Request body.

**Responsibilities:**

* Store structured request information.
* Provide request information to other components.

**Does Not Handle:**

* Network communication.
* Security analysis.
* Request forwarding.

---

### HTTPResponse

**Purpose:**
Represent an HTTP response that will be returned to the client.

**Contains:**

* HTTP status code.
* Headers.
* Response body.

**Responsibilities:**

* Store response information.
* Provide response data to the Network Layer.

**Potential Responses:**

* `200 OK`
* `400 Bad Request`
* `403 Forbidden`
* `404 Not Found`
* `500 Internal Server Error`
* Other responses as required.

---

## 3. Security Pipeline

The Security Pipeline coordinates the processing stages that inspect an incoming request.

### Pipeline

**Purpose:**
Pass an `HTTPRequest` through the configured security processing stages.

**Responsibilities:**

* Maintain the order of security stages.
* Execute each stage.
* Pass request context between stages.
* Stop processing when a blocking condition is reached.
* Produce a final security decision.

**Input:**

* `HTTPRequest`

**Output:**

* Security decision.

**Conceptual Flow:**

```text
HTTPRequest
     |
     v
Security Stage 1
     |
     v
Security Stage 2
     |
     v
Security Stage 3
     |
     v
Security Decision
```

**Dependencies:**

* `Middleware`
* `RequestContext`
* `RuleEngine`

---

### Middleware

**Purpose:**
Represent an individual processing stage within the security pipeline.

**Responsibilities:**

* Receive request context.
* Perform a defined processing task.
* Pass processing to the next stage when appropriate.

Middleware provides a common structure for extending the pipeline.

---

### RequestContext

**Purpose:**
Carry request-related information and processing state through the pipeline.

**Contains:**

* `HTTPRequest`
* Security decision.
* Triggered rule information.
* Processing metadata.

The exact contents may evolve during implementation.

---

## 4. Rule Engine

The Rule Engine evaluates requests using individual security rules.

### Rule

**Purpose:**
Define the common interface for security rules.

**Responsibilities:**

* Inspect request data.
* Determine whether the request violates a specific rule.
* Report the result of the inspection.

**Input:**

* `HTTPRequest`
* Relevant request context.

**Output:**

* Rule result.

A rule should not manage network connections or directly send HTTP responses.

---

### RuleEngine

**Purpose:**
Manage and execute the configured security rules.

**Responsibilities:**

* Register security rules.
* Maintain enabled rules.
* Execute rules against requests.
* Collect rule results.
* Determine whether a request should be blocked.

**Input:**

* `HTTPRequest`

**Output:**

* Security decision.
* Triggered rule information.

**Dependencies:**

* `Rule`

---

### SQLInjectionRule

**Purpose:**
Detect request patterns associated with SQL injection attempts.

**Responsibilities:**

* Inspect relevant request components.
* Identify suspicious SQL injection patterns.
* Return a rule result when a potential attack is detected.

**Does Not Handle:**

* Network communication.
* HTTP response generation.
* Logging directly.

---

### XSSRule

**Purpose:**
Detect request patterns associated with cross-site scripting attempts.

**Responsibilities:**

* Inspect relevant request components.
* Identify suspicious XSS patterns.
* Return a rule result when a potential attack is detected.

---

### PathTraversalRule

**Purpose:**
Detect suspicious path traversal patterns.

**Responsibilities:**

* Inspect request paths and relevant parameters.
* Identify traversal attempts.
* Return a rule result when a potential attack is detected.

---

## 5. Security Decision

### SecurityDecision

**Purpose:**
Represent the result of security inspection.

**MVP Decisions:**

```text
ALLOW
BLOCK
```

**Potential Future Decisions:**

```text
ALLOW
BLOCK
LOG
CHALLENGE
```

The initial implementation will focus on `ALLOW` and `BLOCK`.

---

### RuleResult

**Purpose:**
Represent the result produced by an individual security rule.

**Potential Information:**

* Whether the rule triggered.
* Rule identifier.
* Reason for triggering.
* Severity.
* Additional metadata.

The exact structure will be determined during implementation.

---

## 6. Proxy Layer

### ProxyHandler

**Purpose:**
Forward legitimate requests from the WAF to the backend server and return backend responses to the client.

**Responsibilities:**

* Establish a connection with the backend.
* Forward allowed HTTP requests.
* Receive backend responses.
* Return responses through the WAF to the client.
* Handle backend connection errors.

**Input:**

* Allowed `HTTPRequest`.

**Output:**

* Backend `HTTPResponse`.

**Does Not Handle:**

* Security inspection.
* Security rule execution.
* Determining whether a request is malicious.

---

## 7. Logging

### Logger

**Purpose:**
Record operational and security-related events.

**Responsibilities:**

* Record incoming requests.
* Record allowed requests.
* Record blocked requests.
* Record triggered security rules.
* Record network and backend errors.
* Record application startup and shutdown.

**Potential Log Levels:**

```text
INFO
WARNING
ERROR
DEBUG
```

The Logger records decisions made by other components but does not make security decisions itself.

---

## 8. Configuration

### Config

**Purpose:**
Provide runtime configuration to the WAF.

**Potential Configuration:**

* Listening address.
* Listening port.
* Backend address.
* Backend port.
* Enabled security rules.
* Logging settings.

The MVP may initially use hardcoded values or command-line arguments. Configuration files can be introduced as the project develops.

---

## 9. Component Relationships

The primary relationships between components are:

```text
                    TcpServer
                        |
                        v
                    Connection
                        |
                        v
                   HTTPParser
                        |
                        v
                   HTTPRequest
                        |
                        v
                    Pipeline
                        |
                        v
                   RuleEngine
                        |
                 +------+------+
                 |             |
              ALLOW          BLOCK
                 |             |
                 v             v
           ProxyHandler   HTTPResponse
                 |
                 v
          Backend Server
                 |
                 v
           HTTPResponse
                 |
                 v
               Client
```

Logging receives relevant events from multiple components:

```text
TcpServer ────────┐
HTTPParser ───────┤
RuleEngine ───────┼──> Logger
ProxyHandler ─────┤
WAF Application ──┘
```

Configuration provides settings to components that require runtime configuration:

```text
                    Config
                  /    |    \
                 /     |     \
                v      v      v
          TcpServer  Proxy  RuleEngine
```

---

## 10. Component Design Principles

### Single Responsibility

Each component should have a clearly defined responsibility and avoid taking on responsibilities belonging to other components.

### Loose Coupling

Components should interact through clearly defined interfaces rather than relying heavily on each other's internal implementation.

### Testability

Components should be designed so that important behavior can be tested independently where practical.

### Extensibility

The architecture should allow new security rules and processing stages to be added without significant changes to existing components.

### Resource Ownership

C++ resource ownership should be clearly defined, particularly for sockets, connections, memory, and other resources that require explicit lifecycle management.

### Separation of Security and Networking

Security rules should analyze request data without needing to know how the underlying network connection works.

---

## 11. Implementation Note

The components described above represent the intended architecture rather than a fixed list of classes.

During implementation, a component may be:

* Combined with another component.
* Split into multiple classes.
* Replaced with a different design.
* Simplified for the MVP.

Architectural decisions should be updated as implementation reveals new requirements or constraints.