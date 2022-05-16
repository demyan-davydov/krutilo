Import("env")
env.Append(
  LINKFLAGS=[
    "--specs=nano.specs",
    "--specs=nosys.specs"
  ]
)
