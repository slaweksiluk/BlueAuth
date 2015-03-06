#!/usr/bin/env python

from gi.repository import Gtk
#import Gtk
import cfgManage

class CellRendererToggle:
    def __init__(self):
        window = Gtk.Window()
        window.set_default_size(200, 200)
        
        rows = cfgManage.getList()
        
        liststore = Gtk.ListStore(str, bool, str)
        for act in rows:
            liststore.append([act[0], act[1], act[2]])
        
        treeview = Gtk.TreeView(liststore)
        column_text = Gtk.TreeViewColumn("Users")
        column_toggle = Gtk.TreeViewColumn("Enabled")
        column_mobiles = Gtk.TreeViewColumn("Mobiles")
        treeview.append_column(column_text)
        treeview.append_column(column_toggle)
        treeview.append_column(column_mobiles)
        
        cellrenderer_text = Gtk.CellRendererText()
        column_text.pack_start(cellrenderer_text, False)
        column_text.add_attribute(cellrenderer_text, "text", 0)
        
        cellrenderer_toggle = Gtk.CellRendererToggle()
        column_toggle.pack_start(cellrenderer_toggle, True)
        column_toggle.add_attribute(cellrenderer_toggle, "active",1)
        
#        for act in rows:
#            if act[1] == True:
#            cellrenderer_toggle
        
        cellrenderer_mobiles = Gtk.CellRendererText()
        column_mobiles.pack_start(cellrenderer_mobiles, False)
        column_mobiles.add_attribute(cellrenderer_mobiles, "text", 2)        







#  treeView dla listy telefonow
        #temp_mobile = [('nokia', 'true'), ('samsung','false')]
        temp_mobile = cfgManage.getMobilesValues()
        
        print "All mobile values: " + str(temp_mobile)
        
        liststore2 = Gtk.ListStore(str,bool)
        for i in temp_mobile:
            liststore2.append(i)
        #liststore2.append([('nokia','str')])
        
        treeview2 = Gtk.TreeView(liststore2)
        column_1 = Gtk.TreeViewColumn("ID")
        column_2 = Gtk.TreeViewColumn("Enabled")
#        column_3 = Gtk.TreeViewColumn("Signal power")
        
        treeview2.append_column(column_1)
        treeview2.append_column(column_2)
#        treeview.append_column(column_mobiles)
        
        cellrenderer_1 = Gtk.CellRendererText()
        column_1.pack_start(cellrenderer_1, False)
        column_1.add_attribute(cellrenderer_1, "text", 0)       

#        cellrenderer_2 = Gtk.CellRendererText()
#        column_2.pack_start(cellrenderer_2, False)
#        column_2.add_attribute(cellrenderer_2, "text", 1)  
        cellrenderer_2 = Gtk.CellRendererToggle()
        column_2.pack_start(cellrenderer_2, True)
        column_2.add_attribute(cellrenderer_2, "active",1)


# podlaczanie sygnalow         
        window.connect("destroy", lambda w: Gtk.main_quit())
        cellrenderer_toggle.connect("toggled", self.cell_toggled, liststore)
        cellrenderer_2.connect("toggled", self.cell_toggled, liststore2)               


# przyciski
        button1= Gtk.Button("Add user")
        button2 = Gtk.Button("Delete user")
        button3 = Gtk.Button("Add mobile")
        button4 = Gtk.Button("Delete mobile")
   
   
# szkielet                        
        grid = Gtk.Table(2,2,False)             
        grid.attach(treeview,0,1,0,1)
        grid.attach(button1,0,1,1,2)
        grid.attach(treeview2,1,2,0,1)
        grid.attach(button3,1,2,1,2)
        
        window.add(grid)
        #window.add(treeview)
        window.show_all()
 
 
# funkcja do oblusgi ptaszka dla users i mobiles    
    def cell_toggled(self, widget, path, model):
        model[path][1] = not model[path][1]
        itr = model.get_iter(path)
        val = model.get_value(itr, 1)
        print "Liststore: " + str(val)


CellRendererToggle()
Gtk.main()
