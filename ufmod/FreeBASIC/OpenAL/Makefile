# Target OS: Linux (i386)
# Driver:    OpenAL

play_file, torus: play_file.bas torus.bas oalufmod.o
	fbc play_file.bas -a oalufmod.o
	fbc torus.bas -a oalufmod.o
	strip -R .comment -R .gnu.version play_file
	strip -R .comment -R .gnu.version torus
	# sstrip play_file
	# sstrip torus
