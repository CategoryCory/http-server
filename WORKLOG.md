# Work Log

This document records the engineering decisions behind the project. The
[README.md](README.md) describes how to use the project, and
[CHANGELOG.md](CHANGELOG.md) records externally visible changes.

## How To Use This Log

- Add entries when a phase reaches a meaningful milestone or decision point.
- Focus on reasoning, tradeoffs, validation, and lessons learned rather than a chronological list of edits.
- Link to relevant pull requests, commits, tests, or documentation where useful.

## Phase 1: RAII Socket Abstractions

**Status:** Complete

- What ownership and lifetime problems needed to be solved?
- Why was the chosen RAII boundary appropriate for file descriptors?
- Which copy and move semantics were selected, and why?
- How did tests demonstrate correct cleanup and ownership transfer?
- What would you change if the abstraction needed to support another platform?

## Phase 2: Blocking Single-Client TCP Server

**Status:** Complete

- What server states and lifecycle guarantees did the design need?
- Why begin with a blocking, single-client model?
- How are socket and server errors communicated to callers?
- Which loopback or integration tests validated real connection behavior?
- What limitations does this phase intentionally leave for later?

## Phase 3: Minimal HTTP/1.1 Implementation

**Status:** Planned

- What is the smallest useful HTTP/1.1 request and response feature set?
- Which parts of the protocol will be deliberately out of scope at first?
- How will parsing be separated from socket I/O and response generation?
- Which example requests and tests will define completion for this phase?

## Phase 4: Robust Protocol Handling

**Status:** Planned

- Which malformed, incomplete, or oversized inputs must be handled safely?
- What limits, timeouts, and error responses will be introduced?
- How will the implementation preserve clarity while handling edge cases?
- Which regression tests will protect the protocol boundary?

## Phase 5: Concurrent and Scalable Connection Handling

**Status:** Planned

- What evidence will determine that concurrency is necessary?
- Which connection-handling model is appropriate, and why?
- How will resource ownership and shutdown semantics remain reliable?
- What measurements or load tests will validate the design?

## Phase 6: Production-Oriented Features

**Status:** Planned

- Which production concerns are in scope for this portfolio project?
- What observability, configuration, security, and deployment choices matter?
- Which features are intentionally excluded, and why?
- What would constitute a credible release-ready milestone?
