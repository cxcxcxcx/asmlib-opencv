# Quality is bad #
The library hasn't been updated for a long time. Please consider other libraries like stasm or others.

# Why is it so slow? #
Make sure you do the **"Release"** build. The "debug" build is slow because the heavy use of STL. It should be fast enough for real-time processing.

# Why is the result unstable over frames? #
Each frame is now processed independently. I'm thinking of improving it some time.