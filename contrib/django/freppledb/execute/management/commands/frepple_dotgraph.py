#
# Copyright (C) 2010 by Johan De Taeye
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#

#  file     : $URL$
#  revision : $LastChangedRevision$  $LastChangedBy$
#  date     : $LastChangedDate$

import os
from optparse import make_option

from django.core.management.base import BaseCommand, CommandError
from django.db import transaction, connection
from django.template.loader import render_to_string

from execute.models import log
from input.models import Operation, Resource, Buffer, Load, Flow

#TODO handling of suboperations!!!

@transaction.commit_manually
class Command(BaseCommand):
  help = "Generates output in the DOT language to visualize the network"

  requires_model_validation = False

  @transaction.autocommit
  def handle(self, **options):
    try:
      cursor = connection.cursor()
      
      # Header
      print 'digraph G {'
      print 'rankdir=LR;'
      print 'graph [bgcolor=white];'
      print 'edge [color=black];'
      print 'node[style=filled,fontsize=8,label=""];'
      
      # Buffers
      print 'subgraph buffers {'
      print '  node[shape=triangle,color=red];'
      cursor.execute('select name from buffer')
      for row in cursor.fetchall():
        print '  "B%s" [label="%s",tooltip="%s"];' % (row[0], row[0], row[0]);
      print '}'
      
      # Resources
      print 'subgraph resources {'
      print '	 node[shape=circle,color=blue];'
      cursor.execute('select name from resource')
      for row in cursor.fetchall():
        print '  "R%s" [label="%s",tooltip="%s"];' % (row[0], row[0], row[0]);
      print '}'
            
      # Operations
      # TODO shows only 1 level of suboperations
      print 'subgraph operations {'
      print '	 node[shape=rectangle,color=green];'
      cursor.execute('''
         select name, type, suboperation_id
         from operation
         left join suboperation
         on name = operation_id
         where name not in (select suboperation_id from suboperation)
         order by name, suboperation.priority
         ''')
      previous = None
      needs_closure = 0
      for o, t, s in cursor.fetchall():
        if o != previous and needs_closure > 0:
          needs_closure -= 1
          print '  }'
        if s == None:
          print '  "O%s" [label="%s",tooltip="%s"];' % (o, o, o);
        else:
          if o != previous:
            print '  subgraph "cluster_O%s" {' % o
            print '    label="%s";' % o
            print '    tooltip="%s";' % o
            print '    fontsize=8;'
            previous = o
            needs_closure += 1
          print '    "O%s" [label="%s",tooltip="%s"];' % (s,s,s)           
      print '}'
      
      # Flows      
      print 'subgraph flows {'
      print '  edge[weight=100];'
      cursor.execute('select operation_id, thebuffer_id, quantity from flow')
      for o, b, q in cursor.fetchall():
        if q > 0:
          print '  "O%s"->"B%s";' % (o,b)
        else:
          print '  "B%s"->"O%s";' % (b,o)     
      print '}'
      
      # Loads      
      print 'subgraph loads {'
      print '  edge[style=dashed,dir=none,weight=100];'
      cursor.execute('select operation_id, resource_id from resourceload')
      for o, r in cursor.fetchall():
        print '  "O%s"->"R%s";' % (o,r)
      print '}'
 	    
	    # Footer
      print '}'

    except Exception, e:
      raise CommandError(e)    