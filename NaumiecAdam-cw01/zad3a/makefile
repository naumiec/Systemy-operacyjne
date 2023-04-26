test_static:
	(cd ../zad2; make static)
	../zad2/main < tests.txt > results_static.txt

test_shared:
	(cd ../zad2; make shared)
	../zad2/main < tests.txt > results_shared.txt

test_dynamic:
	(cd ../zad2; make dynamic)
	../zad2/main < tests.txt > results_dynamic.txt

test_all:
	make test_static
	make test_shared
	#make test_dynamic

clean:
	rm -f results_dynamic.txt
	rm -f results_shared.txt
	rm -f results_static.txt
