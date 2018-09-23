#!/usr/bin/env python2

import os
import glob
print 'HTTP/1.1 200 OK'
print 'Content-Type:text/html' # HTML is following
print                          # Leave a blank line
print '<h1>Index of {0}</h1>'.format(os.environ['PATH_INFO'])
print '<p><a href="{0}">{1}</a></p>'.format('/', 'Go to root directory')
print '<ul>'

for f in os.listdir('../htdocs' + os.environ['PATH_INFO']):
    print '<li><a href="{0}">{1}</a></li>'.format(os.environ['PATH_INFO'] + '/' + f, f)

print '</ul>'