require 'sinatra'

set bind: '0.0.0.0', port: 80

get '/courses/ics2016/assessments/bomblab/log_submit' do
  puts params.inspect
  'OK'
end
