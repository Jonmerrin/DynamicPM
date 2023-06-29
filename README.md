*TODO*: Please edit the following information in your final project

## Implementation of Surface Simplification Using Quadric Error Metrics

## https://www.youtube.com/watch?v=qfl6fkYbPXU

![image](https://user-images.githubusercontent.com/2687640/145971368-5c950b12-780b-4325-b117-2f56ef1a1b86.png)
![image](https://user-images.githubusercontent.com/2687640/145971401-86cfb634-c6b3-4e1f-81f8-65c367bc31ad.png)
Left: Bunny decimated to 10% of original triangle count. Right: Bunny at 100% triangle count
![image](https://user-images.githubusercontent.com/2687640/145971990-15005e32-e74e-4dfd-98b0-4310e185fb16.png)
![image](https://user-images.githubusercontent.com/2687640/145972036-c663ee77-937d-4eee-9c57-fc5a8561b3be.png)
Left: Monkey decimated to 50% of original triangle count. Right: Monkey at 100% triangle count
![image](https://user-images.githubusercontent.com/2687640/145972108-0b5f0827-e2c6-4818-a7df-6ff20d8ef9b2.png)
![image](https://user-images.githubusercontent.com/2687640/145972189-a614c2ca-c126-4fdb-b990-239a5584a786.png)
Left: Monkey decimated to 25% of original triangle count. Right: Monkey at 100% triangle count
![image](https://user-images.githubusercontent.com/2687640/145972550-81738619-c25a-4e51-ac12-2e37e9ff8aeb.png)
![image](https://user-images.githubusercontent.com/2687640/145972587-7d59ab54-5100-4d9e-bcb4-f298dbbaf780.png)

Left: Monkey decimated to 12.5% of original triangle count. Right: Monkey at 100% triangle count

![image](https://user-images.githubusercontent.com/2687640/145972350-8037667e-9334-442f-b43d-6034f78f6d6c.png)
![image](https://user-images.githubusercontent.com/2687640/145972384-70b48b23-cf18-4fc7-8c85-e8ee8140b31f.png)
![image](https://user-images.githubusercontent.com/2687640/145972427-d3221877-1777-4987-ae56-f006c375fe1c.png)
![image](https://user-images.githubusercontent.com/2687640/145972454-7fa6d500-c03a-45b0-856a-0091671a22e0.png)

Monkeys at 10% of original triangle count.

HandpaintedTree.obj posed a particular problem, I think because of a combination of the size and the number of non-manifold sections of the mesh. Normally, we could fix tears in the mesh using a higher tolerance level, but due to the size and severity of the tears, the tolerance needed to close them was high enough to cause some unseemly artifacts. Documented below is a couple of tolerances I tried to deal with the problem.

![image](https://user-images.githubusercontent.com/2687640/145974332-11a3ed10-8e0a-454d-a1dc-a85a55df2d4b.png)
80% tolerance = 0. You can immediately start to see some tearing around the box and the base of the tree
![image](https://user-images.githubusercontent.com/2687640/145974428-b44672b3-87a2-44d5-a9c2-a84bee5b31d7.png)
20% tolerance = 0. The tearing intensifies.
![image](https://user-images.githubusercontent.com/2687640/145974495-edcfbe89-a240-4dc5-ba2d-131e2568b586.png)
10% tolerance = 0. Oh god. The humanity.

![image](https://user-images.githubusercontent.com/2687640/145973692-adee1569-9b5f-4b18-a8a3-3453c0cd106a.png)
![image](https://user-images.githubusercontent.com/2687640/145973785-d3813d1a-68f2-4194-8866-9c93b0b6112f.png)
80%, tolerance = 1.1. This manages to prevent the tearing for the first 20% of triangle removal. Not too shabby at all, since practically we'd probably want to keep a lot of the detail.

![image](https://user-images.githubusercontent.com/2687640/145973842-035ccf9a-987a-4140-8a50-7d33d66bb619.png)
![image](https://user-images.githubusercontent.com/2687640/145973903-0780d723-9cf8-4db3-bae4-5bc776670550.png)
50%, tolerance = 1.1. The tearing begins to rear its ugly head. The box also entirely disappears. I imagine that because the box has few triangles and the edges around the tree are now calculating a cumulative sum of the square distances for many triangles after thousands of contractions, at some point the box's error came out as the smallest error despite being visually significant. This is fine when you don't need to simplify this low, but this is a prime example for why being able to "paint" a mesh to keep certain areas from being contracted would be useful for preserving significant features at the extremeties, not only in the case of wanting to preserve detail.

![image](https://user-images.githubusercontent.com/2687640/145973970-31a3829c-fbc3-4ecd-8742-ee8665eda636.png)
![image](https://user-images.githubusercontent.com/2687640/145974038-e515352f-4ce2-4f1b-9c3e-b30d36f68184.png)
20%, tolerance = 1.1. The tearing worsens, but never gets too bad.

![image](https://user-images.githubusercontent.com/2687640/145974085-83fae898-2452-4979-b9dc-fc58adb7bc16.png)
![image](https://user-images.githubusercontent.com/2687640/145974140-54b494b8-3f3e-4bca-88f7-272d835ea302.png)
10%, tolerance = 1.1. Although the tearing is significant, the tree is still entirely held together, which already massively outperforms basic edge contraction.

![image](https://user-images.githubusercontent.com/2687640/145974871-18e9f884-c4ca-418e-a684-59878c25335e.png)
50%, tolerance = 4.0. 4.0 is an extremely high tolerance that will likely cause significant artifacts. Here, it seemed to be the highest I could put it without causing the tree to implode. Even at 4.0, depending on how the priority queue is breaking ties, you can end up with imploded parts of the tree. This tolerance didn't make a significant difference from 1.1 at 50%, but it will show its worth as we decimate more.
![image](https://user-images.githubusercontent.com/2687640/145975007-8ea10d70-8d66-4a3c-819b-38ea1120c51e.png)
30%, tolerance = 4.0. Here we can start to see artifacts caused by the high tolerance, namely a sharp triangle jutting from the leaves into a branch. That's a vertex pair that contract to approximate a triangle from the original mesh that were very far away from each other. This is another good argument for "painting", because we would probably want to contract just the trunk and branches with a high tolerance, and the finer leaves and ends of the branches with a lower tolerance. Overall though, the tearing almost seems to be improving instead of getting worse as the torn parts of the trunk have the opportunity to re-approximate the original mesh through contraction.
![image](https://user-images.githubusercontent.com/2687640/145975100-c00cd46c-1cdf-415e-aea1-a0a9bef1be1c.png)
~10%, tolerance = 4.0
I was honestly thrilled with how 10% looked here. There's still tearing, but clearly the vertex aggregation is putting in the work to prevent it from getting worse. I'm sure with finer tuning and/or painting, this could be much more effective at simplifying this type of mesh.

* Name and partners name(At most teams of 2 folks total)
  * Team member 1: Jonathan Merrin
* How many hours did it take you to complete this final project? 40
* Did you collaborate or share ideas with any other students/TAs/Professors? Nope, unless you count yourself.
* Did you use any external resources? 
  * Note it is fair to utilize some tutorials--cite them here. You **must** add your own personal touch however!
  * http://www.cs.cmu.edu/~garland/Papers/quadrics.pdf
  * https://www.cplusplus.com/
  * https://stackoverflow.com/
  * (Optional) What was the most interesting part of the Final Assignment? How could the instructor improve the final project?
   * Oh man! Just getting to do whatever we wanted with graphics was awesome. It's also incredibly daunting. My project didn't work until around 1 pm the day it was due, which was insanely stressful, because I wasn't sure how you'd grade a project that didn't work without clear milestones. It worked out though, and I'm extremely proud of what I made. The reason I submitted the video so late is because I literally spent 2 hours watching what I made. It's great.

### Rubric

<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>(33.3%) Project Completion</td>
     <td align="left"><ul><li>Does the project compile and run.</li><li>Is it polished without any bugs (No weird visual artifacts).</li><li>Did you make a video?</li><li>Did you add a screenshot of your project to the repository?</li></ul></td>
    </tr>
    <tr>
      <td>(33.3%) Technical</td>
      <td align="left"><ul><li>Was the implementation of the project challenging?</li><li>Even if you followed a tutoral, it should not be trivial, and have some personal touch to it.</li><li>Did you have to organize/process a sufficient amount of data?</li><li>Was it clear you consulted some outside resources that go above and beyond the scope of this class</li></ul></td>
    </tr>
    <tr>
      <td>(33.4%) Creativity</td>
      <td align="left"><ul><li>How visually appealing is the scene?<ul><li>Note: There should be some 'wow' factor--instructors discretion is used here.</li></ul></li><li>How original is the project<ul><li>Again, did you enhance a tutorial and do something unique or just go by the book?</li></ul></li></ul></td>
    </tr>
  </tbody>
</table>
