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
    namespace traits
    {

        template <typename Writer, typename ResultsType> 
            concept HasResultsWriteImpl = requires(Writer w, Results<ResultsType> r){
            { w.write(r) };
        };

        template <typename Writer, typename ResultsType>
        concept CanWriteResults = requires(Writer t, bm::Results<ResultsType> r) {
            requires(requires { Writer::write(r); }) ||
                        (requires { t << r; }) ||
                        (requires(const char *s, std::streamsize n) { Writer::write(s, n); });
        };
    }



    template<typename Writer, typename ResultsType> requires traits::HasResultsWriteImpl<Writer, ResultsType>
    inline void write_results_to(const Results<ResultsType>& t){
        Writer{}.write(t);
    }

}