#!/usr/bin/env python
# author : Power
# file : appmain.py base the base.py

import gtk
class Base(gtk.Window):
    def __init__(self):
        super(Base,self).__init__()

        self.set_title("Base Win")
        self.set_size_request(500,500)
        self.set_position(gtk.WIN_POS_CENTER)

        self.connect("destroy",self.on_hide)

    def on_show(self):
        self.show_all()
    def on_hide(self,widget,data=None):
        self.hide_all()

class PyApp(gtk.Window):
    def __init__(self):
        super(PyApp,self).__init__()
        self.set_title("main win")
        self.set_size_request(300,300)
        self.set_position(gtk.WIN_POS_CENTER)

        self.base = Base()
        button = gtk.Button("show Base")
        button.connect("clicked",self.on_show_basewin)

        self.add(button)
        self.show_all()

        self.connect("destroy",gtk.main_quit)

    def on_show_basewin(self,widget):
        self.base.on_show()

PyApp()
gtk.main()
