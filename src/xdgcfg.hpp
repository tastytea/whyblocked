/*  Copyright © 2019 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the BSD-3-Clause license.
 */

#ifndef XDGCFG_HPP
#define XDGCFG_HPP

#if __cplusplus >= 201703L
    #include <filesystem>
#else
    #include <experimental/filesystem>
#endif
#include <string>
#include <iostream>
#include <cstdint>
#include <libconfig.h++>

#if __cplusplus >= 201703L
    namespace fs = std::filesystem;
#else
    namespace fs = std::experimental::filesystem;
#endif
using std::string;
using std::uint8_t;
using std::cerr;
using std::endl;

class xdgcfg
{
public:
    /*!
     *  @brief  Checks if subdir is present, creates it if necessary
     *
     *          Example:
     *  @code
     *          xdgcfg config("test.cfg", "subdirectory");
     *  @endcode
     *
     *  @param  filename  The name of the file, including extension
     *  @param  subdir    The subdir (optional)
     */
    explicit xdgcfg(const string &filename, const string &subdir = "");

    /*!
     *  @brief  Read the file
     *
     *  @return 0 on success, 1 on I/O error, 2 on parse error.
     */
    uint8_t read();

    /*!
     *  @brief  Write the file
     *
     *  @return `true` on success
     */
    bool write();

    /*!
     *  @brief  Returns a reference to the config as libconfig::Config
     *
     *          Example:
     *  @code
     *          libconfig::Config &cfg = config.get_cfg();
     *  @endcode
     */
    libconfig::Config &get_cfg();

    /*!
     *  @brief  Returns the complete filepath
     */
    const fs::path get_filepath() const;

    /*!
     *  @brief  Sets verbosity
     */
    void set_verbose(bool verbose);

    /*!
     *  @brief  Returns verbosity
     */
    bool get_verbose() const;

private:
    /*!
     *  Holds the contents of the CFG file
     */
    libconfig::Config _cfg;

    /*!
     *  Complete filepath
     */
    fs::path _filepath;

    /*!
     *  Print out error messages if true
     */
    bool _verbose;
};

/*!
 *  @example example.cpp
 */

#endif  // XDGCFG_HPP
