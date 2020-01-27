#pragma once

#include <vector>
#include <cstdint>

#include <string>
#include <memory>

#include <openssl/evp.h>

namespace Luna
{
  /*************************************************************************//**
   * Provide a cryptographic interface for communication and data storage.
   ****************************************************************************/
  class Crypto
  {


    std::unique_ptr<EVP_PKEY> loadPrivateKey(const std::string & path);
    std::unique_ptr<EVP_PKEY> loadPublicKey(const std::string & path);

    public:

    void encrypt(const std::vector<uint8_t> & dataIn,
                 std::vector<uint8_t> & dataOut)
    {

    }

    void decrypt(const std::vector<uint8_t> & dataIn,
                 std::vector<uint8_t> & dataOut)
    {

    }
  };
}
