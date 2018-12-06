#!/usr/bin/ruby
require 'mkmf'

$INCFLAGS << " -I#{Gem::Specification.find_by_name("mysql2").gem_dir}/ext/mysql2"
$INCFLAGS << " -I#{Gem::Specification.find_by_name("glib2").gem_dir}/ext/ext/glib2"
$cppflags = "-lstdc++"

have_type('my_bool', "mysql/mysql.h")

create_makefile('convert_arrow')
