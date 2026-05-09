module modem_top(
    input clk,
    input [3:0] sw,
    input [15:15] sw_mode,
    input [7:0] ja,
    input [3:0] jb,
    input digital_msg_in,
    input drdy,
    input btnC, btnU, btnL, btnR, btnD,
    output uart_tx_out
);

reg [2:0] mode = 0;
reg [7:0] carrier_phase = 0;

wire [11:0] input_signal;
wire [7:0] input_scaled;
wire [7:0] sine_lut;
wire [7:0] mod_out;
wire [7:0] demod_out;
wire [7:0] tx_byte;

assign input_signal = {jb, ja};
assign input_scaled = input_signal[11:4];

assign tx_byte = (sw_mode[15]) ? demod_out : mod_out;

// Mode selection
always @(posedge clk) begin
    if (btnC) mode <= 0;
    else if (btnU) mode <= 1;
    else if (btnL) mode <= 2;
    else if (btnR) mode <= 3;
    else if (btnD) mode <= 4;
end

// Carrier phase update
always @(posedge clk) begin
    case(mode)
        3'd1:
            carrier_phase <= carrier_phase + 8'd10 + input_signal[11:7];

        3'd3:
            carrier_phase <= carrier_phase +
                            (digital_msg_in ? 8'd25 : 8'd10);

        default:
            carrier_phase <= carrier_phase + 8'd10;
    endcase
end

// Sine LUT
sine_lut_generator lut_inst(
    .carrier_phase(carrier_phase),
    .sine_lut(sine_lut)
);

// Modulator
modulator mod_inst(
    .clk(clk),
    .mode(mode),
    .digital_msg_in(digital_msg_in),
    .sine_lut(sine_lut),
    .input_signal(input_signal),
    .mod_out(mod_out)
);

// Demodulator
demodulator demod_inst(
    .clk(clk),
    .mode(mode),
    .input_scaled(input_scaled),
    .demod_out(demod_out)
);

// UART TX
uart_tx uart_inst(
    .clk(clk),
    .tx_byte(tx_byte),
    .uart_tx_out(uart_tx_out)
);

endmodule
