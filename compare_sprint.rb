sprint1, sprint2 = ARGV


times1, times2 = [sprint1, sprint2].map{ |path|
  body = File.open(path) { |f| f.readlines }
    .map{ |s|
      name, ts = s.split(/\s+:\s+/)
      next nil if !ts
      m = ts.match(/((?:\d|\.)+)ms/)
      [name, m[1].to_f]
    }.compact
}

if times1.size != times2.size
  fail "invalid sizes of times"
end

name_size = times1.map{ |a,b| a.size }.max


too_slows = 0
(0...times1.size).each{ |i|
  name, t1 = times1[i]
  t2 = times2[i][1]
  rate = t2 / t1
  klass = case
    when t1 == 0 || t2 == 0
      "\e[90m"
    when [t1, t2].max < 0.2
      "\e[90m"
    when rate >= 20
      # :too_slow
      too_slows += 1
      "\e[41m"
    when rate >= 2
      # :slow
      "\e[91m"
    when rate >= 1
      # :fair
      "\e[93m"
    when rate >= 0.80
      # :mid
      "\e[97m"
    else
      # :fast
      "\e[92m"
  end
  puts sprintf("%s%-*s stl: %8.3fms ft: %8.3fms rate: %1.2fx%s",
    klass,
    name_size, name, t1, t2, rate,
    "\e[m"
  )
}

if too_slows > 0
  fail "too slow!!"
end
