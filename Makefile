all:
	gcc main.c cJSON/cJSON.c -lcurl -lm -o main
