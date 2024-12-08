import sys, wave
import numpy as np
import matplotlib.pyplot as plt

assert(len(sys.argv) >= 2), "Pas d'arguments"

# Lis le fichier WAV
file = wave.open(sys.argv[1], 'r')
fs = file.getframerate()

# Si le fichier est sur 8 ou 16 bits
if file.getsampwidth() == 1:
	signal = np.frombuffer(file.readframes(-1), dtype="uint8")
	signal = np.array(signal - 128, dtype="int8")
elif file.getsampwidth() == 2:
    signal = np.frombuffer(file.readframes(-1), dtype="int16")
else:
    raise RuntimeError("Taille de l'echantillon non pris en charge")

# Graphe
plt.figure(1)
# Si le fichier est mono ou stereo
if file.getnchannels() == 1:
	# Graphe signal
	plot = plt.subplot(211)
	plot.plot(np.arange(len(signal))/fs, signal)
	plot.set_xlabel('Temps (s)')
	plot.set_ylabel('Energie')
	
	# Spectrogramme
	spec = plt.subplot(212)
	spec.specgram(signal, Fs=fs, NFFT=1024)
	spec.set_xlabel('Temps (s)')
	spec.set_ylabel('Frequence (Hz)')
elif file.getnchannels() == 2:
	left, right = signal[0::2], signal[1::2]
	# Graphe signal
	plot = plt.subplot(211)
	plot.plot(np.arange(len(left))/fs, left, label="Gauche")
	plot.plot(np.arange(len(right))/fs, right, label="Droite")
	plot.set_xlabel('Temps (s)')
	plot.set_ylabel('Energie')
	plot.legend()
	
	# Spectrogramme gauche
	spec1 = plt.subplot(212)
	spec1.specgram(left, Fs=fs, NFFT=1024)
	spec1.set_xlabel('Temps (s)')
	spec1.set_ylabel('Frequence (Hz) [G]')
	
	# Spectrogramme doite
	spec2 = plt.subplot(213)
	spec2.specgram(right, Fs=framerate, NFFT=1024)
	spec2.set_xlabel('Temps (s)')
	spec2.set_ylabel('Frequence (Hz) [D]')
else:
	raise RuntimeError("Nombre de cannal non pris en charge")

plt.show()
sys.exit(0)
