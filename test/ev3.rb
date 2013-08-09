assert('ExampleExtension result 123') do
  ExampleExtension.c_method === 123
end

assert('ExampleExtension result 123 (check from C)') do
  ExampleExtensionTest.c_method_test
end
