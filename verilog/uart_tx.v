module uart_tx(
    input clk,
    input [7:0] tx_byte,
    output reg uart_tx_out
);

localparam BAUD_DIV = 868;

reg [15:0] baud_count = 0;
reg [3:0] bit_index = 0;

always @(posedge clk) begin

    if (baud_count < BAUD_DIV - 1)
        baud_count <= baud_count + 1;

    else begin

        baud_count <= 0;

        case(bit_index)

            0: begin
                uart_tx_out <= 0;
                bit_index <= 1;
            end

            1,2,3,4,5,6,7,8: begin
                uart_tx_out <= tx_byte[bit_index-1];
                bit_index <= bit_index + 1;
            end

            9: begin
                uart_tx_out <= 1;
                bit_index <= 0;
            end

        endcase
    end
end

endmodule
