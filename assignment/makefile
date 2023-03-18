CC=gcc
objects = daemon.o backup_dashboard.o check_file_uploads.o collect_reports.o generate_reports.o lock_directories.o sig_handler.o unlock_directories.o update_timer.o messageQueueServer.o
headers = daemon_task.h

daemon : $(objects)
	$(CC) -o daemon $(objects) -lm -lrt

daemon.o : daemon.c $(headers)
	$(CC) -c daemon.c -lrt

backup_dashboard.o : backup_dashboard.c
	$(CC) -c backup_dashboard.c

check_file_uploads.o  :check_file_uploads.c
	$(CC) -c check_file_uploads.c

collect_reports.o : collect_reports.c
	$(CC) -c collect_reports.c

generate_reports.o : generate_reports.c
	$(CC) -c generate_reports.c 

lock_directories.o : lock_directories.c
	$(CC) -c lock_directories.c

messageQueueServer.o : messageQueueServer.c
	$(CC) -c messageQueueServer.c

clean:
	rm daemon $(objects)

run:
	./daemon

