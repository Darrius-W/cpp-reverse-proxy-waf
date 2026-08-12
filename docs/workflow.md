# Workflow

## Normal Request

Client

↓

Connect

↓

Receive TCP request

↓

Parse HTTP

↓

Run security rules

↓

No threats found

↓

Forward request

↓

Receive backend response

↓

Return response

---

## Malicious Request

Client

↓

Receive request

↓

Parse HTTP

↓

Run security rules

↓

Threat detected

↓

Log event

↓

Return 403 Forbidden