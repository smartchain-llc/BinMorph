#pragma once

namespace bm
{

    template <typename T>
    struct Results
    {
        Results(const T &r) : results{std::move(r)} {}
        const T &get() const noexcept { return results; }
        T results;
    };
    template <typename Writer, typename ResultsType>
    concept CanWriteResults = requires(Writer t, bm::Results<ResultsType> r) {
        requires(requires { t.write(r); }) ||
                    (requires { t << r; }) ||
                    (requires(const char *s, std::streamsize n) { t.write(s, n); });
    };

    struct ResultsWriter
    {
        template <
            template <typename> class T1,
            typename T2>
        struct Fun_
        {
            using type = typename T1<T2>::type;
        };

        template <
            typename WriterImpl,
            typename T
        >
            requires CanWriteResults<WriterImpl, bm::Results<T>>
        static void write(const bm::Results<T> &results)
        {
            using ResultsType = decltype(results);
            WriterImpl _w{};
            _w.write(results);
        }
        template <typename WriterImpl, typename T>
            requires CanWriteResults<WriterImpl, T>
        static void write(const WriterImpl &writer, const bm::Results<T> &results)
        {
            writer.write(results);
        }
    };
}