# Target OS: Linux (i386)
# Driver:    OSS

play_file, torus: play_file.bas torus.bas ufmod.o
	fbc play_file.bas -a ufmod.o
	fbc torus.bas -a ufmod.o
	strip -R .comment -R .gnu.version play_file
	strip -R .comment -R .gnu.version torus
	# sstrip play_file
	# sstrip torus
