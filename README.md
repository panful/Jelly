# 介绍
自己写着玩的可视化工具包。

# TODO
- [x] 组件
- [ ] 查找依赖
- [ ] option USE_GLFW_WINDOW
- [ ] 可配置的验证层
- [ ] 可配置的设备扩展
- [ ] Renderer 放在 UI 还是 Core ？
- [ ] Rendering目录新增Vulkan，和Vulkan相关的都放在该目录下，Core不依赖Vulkan任何东西（可以扩展OpenGL等）
- [ ] 隐藏平台相关头文件，自动根据不同平台创建不同类型的窗口
- [x] Pipeline 应该放在 Device or Window
- [ ] Window 中的 Device 调用 Render() 时初始化
- [x] Renderer 是否应该持有 Window 的 weak_ptr，考虑将同一个 Renderer 添加到不同的 Window
- [x] PipelineCache 的 pipeline 改为 unique_ptr
- [x] 使用UniformColor着色时，设置了顶点颜色，顶点着色器警告：inColor没有使用
- [ ] 添加ShaderGeneratorCache，着色器代码相同时不用重复生成SPV
- [ ] 共享vk::Buffer，而不仅仅是共享 DataArray，可以将 DataArray 继承自 Object，使用Changed()，DataArray 内部可以声明一个vk::Buffer类型的成员，这样 DataArray 需要依赖 Device 和 BufferData 可能会比较麻烦
- [ ] 暂时不支持修改 DataSet 的 DataArray 数据，同上一条
- [x] Actor颜色应该使用描述符设置，不能在shader中写死
- [x] shared_from_this 修改为 weak_from_this
- [x] Object类添加 Modified Changed needUpdate 类似变量
- [ ] 窗口较小时，图元某些片段没有像素 commit de5ed317ecd33d57cc4e45c866a6a7aa108f821e 之后才出现这个问题，Viewer 尺寸固定为较大时，这种情况会很少出现
- [x] uniform 使用 CopyToDevice 设置数据时，应该只 mapMemory 一次，保证释放 uniform 之前 unMapMemory 就行
- [ ] 当相机到特殊位置后，actor 还是会被相机近平面裁剪，导致一部分看不到
- [ ] 跟随相机的光照，在某些时候会有颜色值为(1,1,1,,1)的片段，白色的光点
- [x] 参数为 std::nullptr_t 的构造函数标记为 = default
- [ ] DataSet 暂时只支持 float
- [x] Mapper 中的 Drawable 不需要每次 Changed 之后都创建
- [x] 将所有类都继承自 Object 确实没必要继承的都改为私有类
- [x] 所有栈上的成员变量改为 std::unique_ptr
- [x] 所有 Get 函数，如果返回的是成员变量，返回类型都改为 常引用 const&
