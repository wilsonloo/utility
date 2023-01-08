/*
    采用 GNU风格
    主版本号.子版本号[.修正版本号[.编译版本号]]
    Major.Minor.Revision.Build
    Major    主版本号  ：具有相同名称但不同主版本号的程序集不可互换。它适用于对产品的大量重写，或战略性转变，且无法实现向后兼容性。
                         当功能模块有较大的变动，比如增加模块或是整体架构发生变化。【此版本号由项目决定是否修改】。
    Minor    子版本号  ：如果两个程序集的名称和主版本号相同，而次版本号不同，但照顾了向后兼容性。它适用于产品的修正版或完全向后兼容的新版本。【此版本号由项目决定是否修改】。
    Revision 修正版本号：名称、主版本号、次版本号都相同，但修订号不同的程序集应该是完全可以互换的。它适用于修复以前发布的程序集中的安全漏洞。
                         一般是Bug 的修复或是一些小的变动或是一些功能的扩充，要经常发布修订版，修复一个严重 Bug 即可发布一个修订版。【此版本号由项目经理决定是否修改】。
    Build    编译版本号：内部版本号的不同，表示对相同源所做的重新编译。它适合于更改处理器、平台或编译器的情况。
*/
#ifndef EVL_NEXT_GAME_VERSION_BASE_H_
#define EVL_NEXT_GAME_VERSION_BASE_H_

// 主版本号
#define VERSION_MAJOR_EVL			1

// 子版本好
#define VERSION_MINOR_EVL			0

// 修正版本号
#define VERSION_REVISION_EVL		0

// 编译版本号
#define VERSION_BUILD_EVL 			$WCREV$ 

// 是否本地有修改
#define LOCAL_MODIFICATION_FOUND    $WCMODS?true:false$

// 拼接三个元素
#define NUM3STR_EVL(a,b,c)			#a "." #b "." #c

// 拼接四个元素
#define NUM4STR_EVL(a,b,c,d)		#a "." #b "." #c "." #d

// 常规版本号
#define VERSIONSTR_EVL(a,b,c)		NUM3STR_EVL(a,b,c)

// 编译版本号
#define VERSIONBUILDSTR_EVL(a,b,c,d)	NUM4STR_EVL(a,b,c,d)

// 文件版本号
#define FILE_VERSION_STR_EVL		VERSIONBUILDSTR_EVL(VERSION_MAJOR_EVL,VERSION_MINOR_EVL,VERSION_REVISION_EVL,VERSION_BUILD_EVL)

// （正式）产品版本号
#define PRODUCT_VERSION_STR_EVL		VERSIONSTR_EVL(VERSION_MAJOR_EVL,VERSION_MINOR_EVL,VERSION_REVISION_EVL)

// 本文件产生日期时间
#define SRC_TIME_EVL "$WCDATE$"   
#define BUILDING_EVL "$WCNOW$" 

#endif // EVL_NEXT_GAME_VERSION_BASE_H_