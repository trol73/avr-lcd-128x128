name = 'avr-lcd-module-128x128'

simulation = False
#simulation = True

src = [
   'src/lib/uart/*.c',
   'src/*.c',
   'src/asm/*.s'
]

mcu = 'atmega328'

frequency = 20*1000000

port = ''

baudrate = 1200

programmer = 'arduino'

defines = [
   'UART_RX_BUFFER_SIZE=512',
   'UART_TX_BUFFER_SIZE=128',
   'F_CPU=20000000', 'DEBUG=0'
]

compiler_options = ['-g2']

linker_options = []

if simulation:
	compiler = 'gcc'
	libs = 'sdl2 portaudio-2.0 libpng'
	include = ['src/simulator/stubs']
	src.append('src/simulator/*.c');
	src.append('src/simulator/asm/*.c');
	src.append('src/simulator/stubs/*.c');
	defines.append('SIMULATION')
	defines.append('bool=uint8_t')
	defines.append('__AVR_ATmega328__')
	defines.append('true=1')
	defines.append('false=0')

