container: clean 
	g++ -I include/ container.cc -o container 


clean: 
	rm -f container
