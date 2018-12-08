#!/usr/bin/ruby
require 'mkmf'

$INCFLAGS << " -I#{Gem::Specification.find_by_name("mysql2").gem_dir}/ext/mysql2"
$INCFLAGS << " -I#{Gem::Specification.find_by_name("glib2").gem_dir}/ext/glib2"
$INCFLAGS << " -I/usr/local/Cellar/glib/2.58.1/include/glib-2.0"
$INCFLAGS << " -I/usr/local/Cellar/glib/2.58.1/lib/glib-2.0/include"

$CXXFLAGS << " -std=c++11"
$LIBS << " -larrow"

have_type('my_bool', "mysql/mysql.h")

create_makefile('mysql2_arrow')
