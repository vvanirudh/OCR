all: train recog
train: obj/train.o obj/Image.o obj/TrainedData.o obj/Box.o
	g++ -g -o train obj/train.o obj/Image.o obj/TrainedData.o obj/Box.o -lcv -lhighgui -lcvaux -lml -lcxcore
obj/train.o: src/train.cpp
	g++ -g -c src/train.cpp -lcv -lhighgui -lcvaux -lml -lcxcore
recog: obj/recog.o obj/Image.o obj/TrainedData.o obj/Box.o
	g++ -g -o recog obj/recog.o obj/Image.o obj/TrainedData.o obj/Box.o -lcv -lhighgui -lcvaux -lml -lcxcore
obj/recog.o: src/recog.cpp
	g++ -g -c src/recog.cpp -lcv -lhighgui -lcvaux -lml -lcxcore
obj/Image.o: src/Image.cpp src/Image.h
	g++ -g -c src/Image.cpp -lcv -lhighgui -lcvaux -lml -lcxcore
obj/TrainedData.o: src/TrainedData.cpp src/TrainedData.h
	g++ -g -c src/TrainedData.cpp -lcv -lhighgui -lcvaux -lml -lcxcore
obj/Box.o: src/Box.cpp src/Box.h
	g++ -g -c src/Box.cpp -lcv -lhighgui -lcvaux -lml -lcxcore
