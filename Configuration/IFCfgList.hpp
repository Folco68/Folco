#ifndef IFCFGLIST_HPP
#define IFCFGLIST_HPP


class IFCfgList
{
  public:
    static IFCfgList* instance();
    static void       release();

  private:
    static IFCfgList* ifcfglist;
    IFCfgList();
};

#endif // IFCFGLIST_HPP
