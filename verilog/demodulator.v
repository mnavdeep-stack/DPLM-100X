module demodulator(
    input clk,
    input [2:0] mode,
    input [7:0] input_scaled,
    output reg [7:0] demod_out
);

reg [7:0] prev_sample;
reg [19:0] env_detect = 0;
reg [31:0] period_count = 0;
reg [19:0] freq_lpf = 0;
reg [19:0] period_reciprocal;

always @(posedge clk) begin

    prev_sample <= input_scaled;
    period_count <= period_count + 1;

    // Reciprocal approximation
    if (period_count > 256)
        period_reciprocal <= 20'd50;
    else if (period_count > 128)
        period_reciprocal <= 20'd100;
    else if (period_count > 64)
        period_reciprocal <= 20'd200;
    else if (period_count > 32)
        period_reciprocal <= 20'd400;
    else
        period_reciprocal <= 20'd800;

    case(mode)

        // AM / ASK Demodulation
        3'd0, 3'd2: begin

            env_detect <= env_detect -
                         (env_detect >> 3) +

                         (((input_scaled > 128) ?
                         (input_scaled - 128) :
                         (128 - input_scaled)) << 5);

            demod_out <= env_detect[15:8];
        end

        // FM Demodulation
        3'd1: begin

            if (prev_sample < 128 &&
                input_scaled >= 128) begin

                freq_lpf <=
                    (freq_lpf - (freq_lpf >> 3)) +
                    (period_reciprocal << 4);

                period_count <= 0;
            end

            demod_out <= freq_lpf[15:8];
        end

        // FSK / PSK Detection
        3'd3, 3'd4: begin

            demod_out <=
                ((input_scaled > prev_sample ?
                input_scaled - prev_sample :
                prev_sample - input_scaled) > 15)
                ? 255 : 0;
        end

    endcase
end

endmodule
