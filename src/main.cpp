/**
 * @file main.cpp
 * @brief Provides the production entry point for the BinMorph CLI.
 */
#include <exception>
#include <iostream>

#include "binmorph/binmorph.hpp"

/**
 * @brief Runs the BinMorph command-line application.
 *
 * @param[in] argc Argument count supplied by the host process.
 * @param[in] argv Argument vector supplied by the host process.
 * @retval 0 Command completed successfully.
 * @retval 1 Command completed with schema, binary, or interpretation diagnostics.
 * @retval 2 Unhandled application exception reached the process boundary.
 */
int main(int argc, char** argv) {
    try {
        return binmorph::runProduction(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << "binmorph: " << error.what() << "\n";
        return 2;
    }
}
