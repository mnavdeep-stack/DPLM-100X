DPLM-100X is a real-time FPGA-based digital modulation and demodulation system developed using an ESP32 and Nexys FPGA platform to explore practical implementation of modern digital communication systems using embedded processing, FPGA architecture, DDS waveform synthesis, and DSP-based signal processing techniques.

The project combines Direct Digital Synthesis (DDS), Digital Signal Processing (DSP), FPGA-based communication logic, and real-time embedded interfacing into a complete hardware communication platform capable of generating, processing, modulating, demodulating, and monitoring multiple analog and digital communication signals in real time.

The primary objective of this project was to understand how communication systems are implemented at hardware level beyond simulation environments by building a fully functional communication chain using FPGA logic and embedded signal generation. The system supports both analog and digital communication techniques including Amplitude Modulation (AM), Frequency Modulation (FM), Amplitude Shift Keying (ASK), Frequency Shift Keying (FSK), and Phase Shift Keying (PSK).

The ESP32 acts as the real-time waveform generation and acquisition unit. Signal generation is implemented using Direct Digital Synthesis (DDS) techniques with lookup-table based sine wave generation combined with hardware timer interrupt processing for stable waveform synthesis and frequency control. The ESP32 generates carrier and message signals using DAC output while simultaneously performing ADC-based signal acquisition and digital message generation.

The DDS implementation allows accurate carrier generation with configurable frequency control, phase stepping, and waveform stability suitable for real-time communication experiments. Lookup-table synthesis is used to reduce computational overhead while maintaining efficient waveform generation performance. Different modulation schemes are dynamically generated using DSP-style signal processing operations directly inside interrupt-driven routines.

The FPGA section of the project is developed using modular Verilog HDL architecture and implements multiple DSP-oriented signal processing blocks for modulation and demodulation. The FPGA processes incoming sampled data from the ESP32 through PMOD-based parallel communication interfaces and performs real-time digital communication operations using pipelined processing logic.

The FPGA design includes several DSP-related operations such as:
• Envelope detection
• Zero-crossing frequency estimation
• Edge detection algorithms
• Phase-based signal recovery
• Real-time waveform processing
• Timing-aware pipelined arithmetic operations
• Lookup-table based waveform reconstruction
• Frequency estimation logic
• Synchronization and strobe handling

For AM and ASK demodulation, FPGA-based envelope detection algorithms are used to recover the original message signal from the incoming modulated waveform. FM demodulation is implemented using zero-crossing based frequency estimation techniques combined with low-pass style averaging methods for stable demodulated output generation. FSK and PSK detection are implemented using edge detection and phase comparison methods to identify frequency and phase transitions in real time.

The DSP-oriented FPGA architecture includes pipelined arithmetic stages to reduce timing violations and improve real-time processing efficiency. Intermediate DSP calculations are separated into multiple stages to improve FPGA timing closure and maintain stable operation during high-speed signal processing. Lookup-table based sine generation and waveform manipulation are also implemented to reduce hardware complexity while maintaining efficient signal synthesis.

The communication between ESP32 and FPGA is implemented using PMOD-based parallel interfacing with DRDY synchronization signals for real-time sampled data transfer. UART-based output monitoring is included for observing modulation and demodulation outputs and validating communication system performance.

The project demonstrates practical implementation of:
• FPGA-based DSP systems
• DDS waveform synthesis
• Real-time digital communication systems
• Embedded and FPGA interfacing
• FPGA timing-aware design
• Hardware-based modulation and demodulation
• Parallel communication interfaces
• Real-time waveform processing architectures

The project helped in understanding practical FPGA development concepts including DSP pipelining, timing synchronization, hardware signal processing, waveform stability, communication latency, FPGA timing optimization, and embedded-FPGA coordination during real-time operation.

Applications of this project include:
• FPGA DSP systems
• Digital communication platforms
• Embedded-FPGA signal processing
• SDR and communication prototyping
• Real-time hardware communication systems
• Educational DSP and communication demonstrations
• FPGA-based waveform processing systems

Future improvements planned for the project include SDR integration, advanced DSP filtering architectures, adaptive demodulation algorithms, higher sampling rate DDS systems, FFT-based spectrum analysis, improved carrier recovery methods, and real-time signal visualization interfaces.
