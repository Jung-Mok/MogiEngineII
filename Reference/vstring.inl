namespace CL
{

template< typename T >
vstring::vstring(T const& rhs)
{
    operator=(rhs);
}

template< typename T >
vstring::vstring(T const* const& rhs)
{
    operator=(rhs);
}

template<typename ... Args>
vstring::vstring(std::string const& format, Args ... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size > 0)
    {
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args ...);
        operator=(std::string(buf.get(), buf.get() + size - 1)); // We don't want the '\0' inside
    }
}

inline std::wstring const& vstring::Get()const
{
    return m_wstr;
}

inline std::wstring& vstring::Get()
{
    return m_wstr;
}

template<typename T_STRING, typename T_CONT >
void vstring::CutTextByKey(T_STRING const& kKey, T_CONT& rOut, bool const bEmptyLineUse)
{
    CutTextByKey(m_wstr, kKey, rOut, bEmptyLineUse);
}

template<typename T_STRING, typename T_CONT >
void vstring::CutTextByKey(T_STRING const& kOrg, T_STRING const& kKey, T_CONT& rOut, bool const bEmptyLineUse)
{
    T_STRING::size_type start_pos = 0;
    while(start_pos != T_STRING::npos)
    {
        T_STRING::size_type const find_pos = kOrg.find( kKey.c_str(), start_pos);

        T_STRING strCut;
        if(find_pos != T_STRING::npos)
        {
            strCut = kOrg.substr(start_pos, find_pos - start_pos);
            start_pos = find_pos+kKey.length();
        }
        else
        {
            strCut = kOrg.substr(start_pos);
            start_pos = T_STRING::npos;
        }

        if(bEmptyLineUse || !strCut.empty())
        {
            rOut.push_back(T_CONT::value_type(strCut));
        }
    }
}

template<typename T_STRING, typename T_CONT>
void vstring::CutTextByKey(T_STRING const& kOrg, T_STRING const& kKey, T_CONT& rOut, T_STRING const& BundleS, T_STRING const& BundleE, bool const bEmptyLineUse)
{
    T_STRING::size_type start_pos = 0;
    while (start_pos != T_STRING::npos)
    {
        T_STRING strCut;
        if (kOrg.find(BundleS, start_pos) == start_pos)
        {
            T_STRING::size_type const find_pos = kOrg.find(BundleE.c_str(), start_pos + BundleS.length());
            if (find_pos != T_STRING::npos)
            {
                //strCut = kOrg.substr(start_pos, find_pos - start_pos + BundleE.length()); //Bundle포함
                strCut = kOrg.substr(start_pos + BundleS.length(), find_pos - start_pos - BundleE.length()); //Bundle미포함
                start_pos = find_pos + kKey.length() + BundleE.length();
                if (kOrg.size() <= start_pos)
                {
                    start_pos = T_STRING::npos;
                }
            }
        }

        if (strCut.empty())
        {
            T_STRING::size_type const find_pos = kOrg.find(kKey.c_str(), start_pos);

            //T_STRING strCut;
            if (find_pos != T_STRING::npos)
            {
                strCut = kOrg.substr(start_pos, find_pos - start_pos);
                start_pos = find_pos + kKey.length();
            }
            else
            {
                strCut = kOrg.substr(start_pos);
                start_pos = T_STRING::npos;
            }
        }

        if (bEmptyLineUse || !strCut.empty())
        {
            rOut.push_back(T_CONT::value_type(strCut));
        }
    }
}

template<typename T_STRING >
T_STRING vstring::CropTextByKey(T_STRING const& strSrc, typename T_STRING::value_type const& cKey, INT32 const& iAddChar)
{
    auto itor = strSrc.find(cKey);
    if (T_STRING::npos == itor)
    {
        return strSrc;
    }
    UINT32 const iCropSize = itor + iAddChar;
    auto const strRet = strSrc.substr(0, iCropSize);
    return strRet;
}

template<typename T_STRING>
void vstring::ConvToUPR(T_STRING& wstrSrc)
{
    std::transform( wstrSrc.begin(), wstrSrc.end(), wstrSrc.begin(), towupper );
}

template<typename T_STRING>
void vstring::ConvToLWR(T_STRING& wstrSrc)
{
    std::transform( wstrSrc.begin(), wstrSrc.end(), wstrSrc.begin(), towlower );
}

template< typename T_STRING, typename T_REPLACE >
void vstring::Replace( T_STRING const& kFindStr, T_REPLACE const& kReplaceData )
{
    Replace(m_wstr, kFindStr, kReplaceData);
}

template< typename T_STRING, typename T_REPLACE >
void vstring::Replace( T_STRING& kOrg, T_STRING const& kFindStr, T_REPLACE const& kReplaceData )
{
    vstring kReplaceStr( kReplaceData );
    auto const next = kReplaceStr.size() - kFindStr.size() + 1;

    auto pos_ = kOrg.find( kFindStr );
    while( pos_ != T_STRING::npos )
    {
        kOrg.replace((T_STRING::size_type)pos_, (T_STRING::size_type)kFindStr.size(), ((T_STRING)(kReplaceStr)).c_str());
        pos_ += next;
        pos_ = kOrg.find( kFindStr, pos_ );
    }
}

template< typename T >
inline vstring& vstring::operator = (Point2<T> const& rhs)
{
    *this = rhs.x;
    *this << L", ";
    *this << rhs.y;

    return *this;
}

template< typename T >
inline vstring& vstring::operator = (Point3<T> const& rhs)
{
    *this = rhs.x;
    *this << L", ";
    *this << rhs.y;
    *this << L", ";
    *this << rhs.z;

    return *this;
}

template< typename T1 >
inline vstring& vstring::operator += (T1 const& rhs)
{
    vstring kTemp(rhs);
    m_wstr += kTemp.m_wstr;
    return *this;
}

template< typename T1 >
inline vstring& vstring::operator += (T1 const* const& rhs)
{
    vstring kTemp(rhs);
    m_wstr += kTemp.m_wstr;
    return *this;
}

template<typename T>
inline vstring& vstring::operator <<(T const& rhs)
{//+= 과 같은역할
    return operator +=(rhs);
}

template< >
inline vstring& vstring::operator += (vstring const& rhs)
{
    m_wstr += rhs.m_wstr;
    return *this;
}

template< >
inline vstring& vstring::operator += (char const* const& rhs)
{
    if (rhs)
    {
        m_wstr += ConvToUNI(rhs);
    }
    return *this;
}

template< >
inline vstring& vstring::operator += (wchar_t const* const& rhs)
{
    if (rhs)
    {
        m_wstr += rhs;
    }
    return *this;
}

template< >
inline vstring& vstring::operator <<(vstringReplace const& kReplaceStr)
{
    Replace(m_wstr, kReplaceStr.m_kFindStr, kReplaceStr.m_kReplaceStr);
    return*this;
}

template<typename T>
inline vstring& vstring::operator <<(T const* const& rhs)
{//+= 과 같은역할
    return operator +=(rhs);
}

template< typename T1 >
inline vstring vstring::operator + (T1 const& rhs)const
{
    vstring org(*this);
    vstring kTemp(rhs);
    org += kTemp;
    return org;
}

template< typename T1 >
inline vstring vstring::operator + (T1 const* const& rhs)const
{
    vstring org(*this);
    vstring kTemp(rhs);
    org += kTemp;
    return org;
}

///

template< typename T >
vstring::operator Point2<T> const() const throw()
{
    std::list< std::wstring > kOut;
    CutTextByKey(m_wstr, std::wstring(L","), kOut);

    Point2<T> outPt;

    auto cut_itor = kOut.begin();

    for(int i = 0; kOut.end() != cut_itor; ++cut_itor, ++i)
    {
		Trim(*cut_itor);
        vstring kCutStr(*cut_itor);
        switch(i)
        {
        case 0:{outPt.x = kCutStr;}break;
        case 1:{outPt.y = kCutStr;}break;
        default:
            {
                return Point2<T>();
            }break;
        }
    }

    return outPt;
}

template< typename T >
vstring::operator Point3<T> const() const throw()
{
    std::list< std::wstring > kOut;
    CutTextByKey(m_wstr, std::wstring(L","), kOut);

    Point3<T> outPt;

    auto cut_itor = kOut.begin();

    for(int i = 0; kOut.end() != cut_itor; ++cut_itor, ++i)
    {
		Trim(*cut_itor);
        vstring kCutStr(*cut_itor);
        switch(i)
        {
        case 0:{outPt.x = kCutStr;}break;
        case 1:{outPt.y = kCutStr;}break;
        case 2:{outPt.z = kCutStr;}break;
        default:
            {
                return Point3<T>();
            }break;
        }
    }

    return outPt;
}

template< >
void vstring::Trim(std::string& kOrg)
{
    static std::string const drop = " \a\b\f\n\r\t\v";

    kOrg.erase(0,kOrg.find_first_not_of(drop));
    kOrg.erase(kOrg.find_last_not_of(drop)+1); 
}

template< >
void vstring::Trim(std::wstring& kOrg)
{
    static std::wstring const drop = L" \a\b\f\n\r\t\v";

    kOrg.erase(0,kOrg.find_first_not_of(drop));
    kOrg.erase(kOrg.find_last_not_of(drop)+1); 
}

}//namespace CL

