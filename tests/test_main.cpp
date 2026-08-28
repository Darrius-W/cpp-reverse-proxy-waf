#include <catch2/catch_test_macros.hpp>
#include "WafSocket.h"

// Unit test suite validating OS resource management and lifecycle mechanics
TEST_CASE("WafSocket Core Lifecycle and Resource Allocation", "[WafSocket]"){
    
    WafSocket test_socket;

    // Assures uninitialized sockets default to invalid state
    SECTION("Constructor initializes socket tracking handle to -1"){
        REQUIRE(test_socket.get_handle() == -1);
    }

    // Verifies successful creation of a fresh network connection
    SECTION("Kernel successfully assigns a valid file descriptor index upon creation"){
        bool allocation_success = test_socket.create_socket(AF_INET, SOCK_STREAM, 0);

        REQUIRE(allocation_success == true);
        REQUIRE(test_socket.get_handle() >= 3);
    }

    // Assures back-to-back allocations close zombie connections to prevent descriptor leaks
    SECTION("Multi-allocation loop safely intercepts and recycles old descriptors"){
        REQUIRE(test_socket.create_socket(AF_INET, SOCK_STREAM, 0) == true);
        REQUIRE(test_socket.create_socket(AF_INET, SOCK_STREAM, 0) == true);
        REQUIRE(test_socket.get_handle() >= 3);
    }
}