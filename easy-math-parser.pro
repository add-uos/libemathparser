TEMPLATE = subdirs

SUBDIRS += parser_qt \
           parser_std \
           muparser_engine \
           examples

examples.depends = parser_qt
examples.depends = parser_std
examples.depends = muparser_engine
