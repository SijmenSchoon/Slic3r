#ifndef ZIPPER_HPP
#define ZIPPER_HPP

#include <string>
#include <memory>

namespace Slic3r {

// Class for creating zip archives.
class Zipper {
public:
    // Three compression levels supported
    enum e_compression {
        NO_COMPRESSION,
        FAST_COMPRESSION,
        TIGHT_COMPRESSION
    };

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
    std::string m_data;
    std::string m_entry;
    e_compression m_compression;

public:

    // Will blow up in a runtime exception if the file cannot be created.
    explicit Zipper(const std::string& zipfname,
                    e_compression level = NO_COMPRESSION);
    ~Zipper();

    // No copies allwed, this is a file resource...
    Zipper(const Zipper&) = delete;
    Zipper& operator=(const Zipper&) = delete;

    // Moving is fine.
    // Zipper(Zipper&&) = default;
    // Zipper& operator=(Zipper&&) = default;
    // All becouse of VS2013:
    Zipper(Zipper &&m);
    Zipper& operator=(Zipper &&m);

    /// Adding an entry means a file inside the new archive. Name param is the
    /// name of the new file. To create directories, append a forward slash.
    /// The previous entry is finished (see finish_entry)
    void add_entry(const std::string& name);

    /// Add a new binary file entry with an instantly given byte buffer.
    void add_entry(const std::string& name, const std::uint8_t* data, size_t l);

    // Writing data to the archive works like with standard streams. The target
    // within the zip file is the entry created with the add_entry method.

    // Template taking only arithmetic values, that std::to_string can handle.
    template<class T> inline
    typename std::enable_if<std::is_arithmetic<T>::value, Zipper&>::type
    operator<<(T &&val) {
        return this->operator<<(std::to_string(std::forward<T>(val)));
    }

    // Template applied only for types that std::string can handle for append
    // and copy. This includes c style strings...
    template<class T> inline
    typename std::enable_if<!std::is_arithmetic<T>::value, Zipper&>::type
    operator<<(T &&val) {
        if(m_data.empty()) m_data = std::forward<T>(val);
        else m_data.append(val);
        return *this;
    }

    /// Finishing an entry means that subsequent writes will no longer be
    /// appended to the previous entry. They will be written into the internal
    /// buffer and ones an entry is added, the buffer will bind to the new entry
    /// If the buffer was written, but no entry was added, the buffer will be
    /// cleared after this call.
    void finish_entry();

    /// Gets the name of the archive without the path or extension.
    std::string get_name() const;

    void close();
};


}

#endif // ZIPPER_HPP
