server-wrapper:
	$(CC) -static run.c -o run

clean:
	$(RM) run
