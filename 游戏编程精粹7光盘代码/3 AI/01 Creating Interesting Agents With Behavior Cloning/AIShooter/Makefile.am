INCLUDES = -I$(top_srcdir)/DecisionTree
bin_PROGRAMS = aishooter learn
aishooter_SOURCES = main.cpp AIShooter.cpp Agent.cpp Ship.cpp Background.cpp Bullet.cpp
aishooter_LDADD = $(top_srcdir)/DecisionTree/libDecisionTree.a -lGL -lGLU -lglut -llua
learn_SOURCES = Learn.cpp TreeWriter.cpp
learn_LDADD = $(top_srcdir)/DecisionTree/libDecisionTree.a
