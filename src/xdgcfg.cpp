/*  Copyright © 2019 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the BSD-3-Clause license.
 */

#include <basedir.h>
#include "xdgcfg.hpp"

xdgcfg::xdgcfg(const string &filename, const string &subdir)
: _cfg()
, _verbose(false)
{
    xdgHandle xdg;
    xdgInitHandle(&xdg);
    _filepath = xdgConfigHome(&xdg);
    xdgWipeHandle(&xdg);

    if (!subdir.empty())
    {
        _filepath /= subdir;
    }
    if (!fs::exists(_filepath))
    {
        fs::create_directories(_filepath);
    }
    _filepath /= filename;
}

uint8_t xdgcfg::read()
{
    try
    {
        _cfg.readFile(_filepath.c_str());
    }
    catch (const libconfig::FileIOException &e)
    {
        if (_verbose)
        {
            cerr << "I/O error while reading " << _filepath
                 << " - " << e.what() << endl;
        }
        return 1;
    }
    catch (const libconfig::ParseException &e)
    {
        if (_verbose)
        {
            cerr << "Parse error at " << e.getFile() << ":" << e.getLine()
                 << " - " << e.getError() << endl;
        }
        return 2;
    }

    return 0;
}

bool xdgcfg::write()
{
    try
    {
        _cfg.writeFile(_filepath.c_str());
    }
    catch (const libconfig::FileIOException &e)
    {
        if (_verbose)
        {
            cerr << "I/O error while writing " << _filepath
                 << " - " << e.what() << endl;
        }
        return false;
    }

    return true;
}

libconfig::Config &xdgcfg::get_cfg()
{
    return _cfg;
}

const fs::path xdgcfg::get_filepath() const
{
    return _filepath;
}

void xdgcfg::set_verbose(bool verbose)
{
    _verbose = verbose;
}

bool xdgcfg::get_verbose() const
{
    return _verbose;
}
