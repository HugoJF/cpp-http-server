#!/usr/bin/env python2

import os
import glob
print 'HTTP/1.1 200 OK'
print 'Content-Type:text/html' # HTML is following
print                          # Leave a blank line
print '<h1>DIR Script2</h1>'
print '<p><a href="{0}">-> {1}</a></p>'.format('/', 'Root')
for f in os.listdir('../htdocs' + os.environ['PATH_INFO']):
    print '<p><a href="{0}">-> {1}</a></p>'.format(os.environ['PATH_INFO'] + '/' + f, f)
print '<p>ScriptName: {0}</p>'.format(os.environ['PATH_INFO'])
print '<h2>PATATA</h2>'