const std = @import("std");
const print = std.debug.print;

pub fn main() !void {
    const stdin = std.io.getStdIn().reader();

    var inputBuffer: [1024]u8 = undefined;
    var countFullyContains: i32 = 0;

    while (try stdin.readUntilDelimiterOrEof(&inputBuffer, '\n')) |line| {
        var t = std.mem.tokenize(u8, line, "-,");

        const start1 = try std.fmt.parseInt(i32, t.next() orelse return error.MissingStartIndex, 10);
        const end1 = try std.fmt.parseInt(i32, t.next() orelse return error.MissingEndIndex, 10);

        const start2 = try std.fmt.parseInt(i32, t.next() orelse return error.MissingStartIndex, 10);
        const end2 = try std.fmt.parseInt(i32, t.next() orelse return error.MissingEndIndex, 10);

        if ((start1 >= start2 and end1 <= end2) or (start2 >= start1 and end2 <= end1)) {
            countFullyContains += 1;
        }
    }

    print("{d}\n", .{countFullyContains});
}
