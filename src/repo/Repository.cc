#include "Repository.hh"

GrailRepository::GrailRepository(uint32_t size) {
  std::vector<uint8_t> bytes(size, 0);
}

GrailRepository::~GrailRepository() {}

void GrailRepository::generateKey(const std::string& regionName,
                                  const std::string& siteName) {
  AESEncDec cipher(password);

  // Couple each site with its keypair.
  for (auto region : siteKeys) {
    if (region.first == regionName) {
      region.second[siteName] = &cipher;
      return;
    }
  }
  siteKeys.push_back(std::make_pair(
      regionName, std::unordered_map<std::string, AESEncDec*>()));
}

void GrailRepository::deleteKey(const std::string& siteName) {
  for (auto region : siteKeys) {
    for (auto site : region.second) {
      if (site.first == siteName) {
        region.second.erase(site.first);
        return;
      }
    }

    std::cerr << "Error: Site: " << siteName << " not in site list.\n";
  }
}

void GrailRepository::backupToCloud(const std::string& service_name,
                                    const std::string& userid,
                                    const std::string& passwd, uint32_t offset,
                                    uint32_t stride, std::string& img_name) {
  SteganographicImage steg(img_name, offset, stride);

  steg.hide_secret(bytes);
  steg.write_webp(img_name);
  client.upload(img_name);
};

void GrailRepository::restoreFromCloud(const std::string& service_name,
                                       const std::string& userid,
                                       const std::string& passwd,
                                       uint32_t offset, uint32_t stride,
                                       std::string& img_name) {
  client.download(img_name, "");
  SteganographicImage steg(img_name, offset, stride);

  bytes = steg.recover();

  std::string base64 = base64_encode(bytes.data(), bytes.size());
  std::cout << base64 << std::endl;
}

void GrailRepository::unlock(const std::string& password, const Factor2& factor,
                             const std::string& area) {
  this->password = password;
  AESEncDec cipher(password);
  cipher.decrypt_file(area.c_str(), "tmp");
};

void GrailRepository::scramble(const std::string& area) {
  std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t>
      rbe;
  std::generate(bytes.begin(), bytes.end(), rbe);
}

void GrailRepository::destroy() { std::fill(bytes.begin(), bytes.end(), 0); }

// PubKey getPublicKey(const std::string& password, const Factor2& factor,
// const std::string& area)
// }

// akin to gpg -s && gpg --verify
uint64_t GrailRepository::proveIdentity(const std::string& password,
                                        uint64_t nonce,
                                        const std::string& area) {
  return 0;
}

AESEncDec GrailRepository::get(const std::string& area, const std::string& name,
                               std::string& userid, std::string& passwd) {
  for (auto region : siteKeys)
    if (region.first == area) return *region.second[name];
  return AESEncDec(NULL);
};

// TODO: Require hardware authentication (à la YubiKey)?
void GrailRepository::recover2ndFactor(){};
