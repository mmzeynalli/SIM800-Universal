# SIM800 Universal Communication library

Library is built on this API: https://github.com/aspenforest/SIM800

A comprehensive SIM800 Series library for simplified communication.

This library provides wrapper methods for all AT commands defined in the official SIM800 manual, many helper methods to work with returned replies, advanced serial timeout functionalities, and also methods to directly communicate with the chip. Full library documentation and numerous examples are also provided.

Library is simplified in such way that, one can use it even without reading whole SIM800 documentation. The old logic was not modified, i.e. one can use API methods directly (bypassing communication protocol wrappers) if they want.

Currently this library supports protocols: HTTP (GET, POST)