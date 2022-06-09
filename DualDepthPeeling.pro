TEMPLATE = subdirs

SUBDIRS = \
    Gui \
    DataModel \
    App

App.file = App/DualDepthPeelingApp.pro
App.depends = DataModel Gui
