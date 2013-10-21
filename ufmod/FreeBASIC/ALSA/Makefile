# Target OS: Linux (i386)
# Driver:    ALSA

play_file, torus: play_file.bas torus.bas alsaufmod.o
	fbc play_file.bas -a alsaufmod.o -l asound
	fbc torus.bas -a alsaufmod.o -l asound
	strip -R .comment -R .gnu.version play_file
	strip -R .comment -R .gnu.version torus
	# sstrip play_file
	# sstrip torus
