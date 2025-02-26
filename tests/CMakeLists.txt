find_package(cmocka CONFIG REQUIRED)
target_link_libraries(main PRIVATE cmocka::cmocka)

# Testleri derliyoruz
add_executable(GetInsertIndexTest GetInsertIndexTest.c)

# Teste kitaplığı linkliyoruz
target_link_libraries(GetInsertIndexTest KOKESort CMocka)

# Testleri çalıştırılabilir hale getiriyoruz
add_test(NAME GetInsertIndexTest COMMAND GetInsertIndexTest)