require "CSV"
require "descriptive_statistics"

array = Array.new(18) {|i| []}

CSV.foreach("./prob_128_loop_special") do |row|
  row[0].split(" ").each_with_index do |cycle, i|
    array[i] << cycle.to_f
  end
end

array.each_with_index do |sub_array, i|
  puts "#{sub_array.sort[499]} #{sub_array.sort[499] * 1 / 2.6} #{sub_array.standard_deviation}"
end