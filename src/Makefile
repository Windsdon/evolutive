sim:
	g++ -std=c++0x main.cpp  Robot.cpp Action.cpp Behaviour.cpp RobotDescriptor.cpp -I "/usr/local/include/player-3.0" -lplayerc++

generator:
	g++ -std=c++0x robotGenerator.cpp Robot.cpp Action.cpp Behaviour.cpp RobotDescriptor.cpp -I "/usr/local/include/player-3.0" -lplayerc++ -o generator.out

selector:
	g++ -std=c++0x selector.cpp Robot.cpp Action.cpp Behaviour.cpp RobotDescriptor.cpp -I "/usr/local/include/player-3.0" -lplayerc++ -o selector.out

remake:
	g++ -std=c++0x remake.cpp -o remaker
